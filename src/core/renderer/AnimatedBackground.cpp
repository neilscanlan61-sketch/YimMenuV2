#include "AnimatedBackground.hpp"
#include <wincodec.h>
#include <propvarutil.h>
#include <cstring>
#include <stdexcept>

namespace YimMenu
{
    namespace
    {
        void Check(HRESULT hr, const char* message) { if (FAILED(hr)) throw std::runtime_error(message); }
        struct Apartment
        {
            HRESULT Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            Apartment() { if (FAILED(Result) && Result != RPC_E_CHANGED_MODE) throw std::runtime_error("Cannot initialize GIF decoder COM apartment."); }
            ~Apartment() { if (SUCCEEDED(Result)) CoUninitialize(); }
        };
        unsigned Metadata(IWICMetadataQueryReader* reader, const wchar_t* name, unsigned fallback)
        {
            PROPVARIANT value{};
            unsigned result = fallback;
            if (reader && SUCCEEDED(reader->GetMetadataByName(name,&value)))
            {
                if (value.vt == VT_UI1) result = value.bVal;
                else if (value.vt == VT_UI2) result = value.uiVal;
                else if (value.vt == VT_UI4) result = value.ulVal;
            }
            PropVariantClear(&value);
            return result;
        }
        D3D12_HEAP_PROPERTIES Heap(D3D12_HEAP_TYPE type)
        {
            D3D12_HEAP_PROPERTIES value{}; value.Type=type; value.CreationNodeMask=1; value.VisibleNodeMask=1; return value;
        }
        Animation::Clip Decode(const std::filesystem::path& path)
        {
            if (!std::filesystem::is_regular_file(path) || std::filesystem::file_size(path)>16ULL*1024*1024)
                throw std::runtime_error("GIF is missing or exceeds 16 MiB compressed limit.");
            Apartment apartment;
            Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
            Check(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory)),"Cannot create WIC decoder.");
            Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
            Check(factory->CreateDecoderFromFilename(path.c_str(),nullptr,GENERIC_READ,WICDecodeMetadataCacheOnDemand,&decoder),"Cannot decode GIF file.");
            GUID container{}; Check(decoder->GetContainerFormat(&container),"Cannot identify GIF container.");
            if (!IsEqualGUID(container,GUID_ContainerFormatGif)) throw std::runtime_error("Background must be a GIF, not just a renamed image.");
            UINT count=0; Check(decoder->GetFrameCount(&count),"Cannot read GIF frames.");
            Microsoft::WRL::ComPtr<IWICMetadataQueryReader> metadata;
            Check(decoder->GetMetadataQueryReader(&metadata),"Cannot read GIF dimensions.");
            Animation::Clip clip;
            clip.Width=Metadata(metadata.Get(),L"/logscrdesc/Width",0);
            clip.Height=Metadata(metadata.Get(),L"/logscrdesc/Height",0);
            Animation::ValidateDimensions(clip.Width,clip.Height,count);
            Animation::Canvas canvas(clip.Width,clip.Height); clip.Frames.reserve(count);
            for (UINT i=0;i<count;++i)
            {
                Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
                Check(decoder->GetFrame(i,&frame),"Cannot read GIF frame.");
                Microsoft::WRL::ComPtr<IWICMetadataQueryReader> frameMetadata;
                Check(frame->GetMetadataQueryReader(&frameMetadata),"Cannot read GIF frame metadata.");
                UINT w=0,h=0; Check(frame->GetSize(&w,&h),"Cannot read GIF frame size.");
                if (!w || !h || w>clip.Width || h>clip.Height) throw std::runtime_error("GIF subframe dimensions are invalid.");
                const unsigned left=Metadata(frameMetadata.Get(),L"/imgdesc/Left",0);
                const unsigned top=Metadata(frameMetadata.Get(),L"/imgdesc/Top",0);
                const unsigned disposal=Metadata(frameMetadata.Get(),L"/grctlext/Disposal",0);
                const unsigned delay=std::clamp(Metadata(frameMetadata.Get(),L"/grctlext/Delay",10),2U,1000U)*10;
                Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
                Check(factory->CreateFormatConverter(&converter),"Cannot create GIF pixel converter.");
                Check(converter->Initialize(frame.Get(),GUID_WICPixelFormat32bppRGBA,WICBitmapDitherTypeNone,nullptr,0,WICBitmapPaletteTypeCustom),"Cannot convert GIF pixels.");
                std::vector<std::uint8_t> rgba(std::size_t(w)*h*4);
                Check(converter->CopyPixels(nullptr,w*4,static_cast<UINT>(rgba.size()),rgba.data()),"Cannot copy GIF frame pixels.");
                clip.Frames.push_back(canvas.Push(left,top,w,h,rgba,disposal,delay));
            }
            return clip;
        }
    }
    void AnimatedBackground::Reset()
    {
        m_Texture.Reset(); m_Uploads.clear(); m_Clip={}; m_Handle={}; m_InShaderState=false;
        m_FailedGpu=false; m_LastFrame=std::numeric_limits<std::size_t>::max(); m_ElapsedMs=0; m_LastTick=std::chrono::steady_clock::now();
        m_Status="No animated background loaded.";
    }
    bool AnimatedBackground::Load(const std::filesystem::path& path)
    {
        Reset();
        try
        {
            m_Clip=Decode(path);
            m_Status="Loaded "+path.filename().string()+" ("+std::to_string(m_Clip.Frames.size())+" frames).";
            m_LastTick=std::chrono::steady_clock::now(); return true;
        }
        catch (const std::exception& e) { m_Status=e.what(); return false; }
    }
    void AnimatedBackground::Tick(bool playing,float speed)
    {
        const auto now=std::chrono::steady_clock::now();
        if (playing) m_ElapsedMs+=std::chrono::duration<double,std::milli>(now-m_LastTick).count()*std::clamp(speed,0.1f,3.0f);
        m_LastTick=now;
    }
    bool AnimatedBackground::Prepare(ID3D12Device* device,ID3D12GraphicsCommandList* commands,
        unsigned frameIndex,unsigned bufferCount,D3D12_CPU_DESCRIPTOR_HANDLE cpu,D3D12_GPU_DESCRIPTOR_HANDLE gpu)
    {
        if (m_FailedGpu || m_Clip.Frames.empty() || !device || !commands || frameIndex>=bufferCount) return false;
        // The render queue serializes writes and draws of this single texture.
        // Each backbuffer has a staging buffer, reused only after its frame fence.
        try
        {
            if (!m_Texture)
            {
                D3D12_RESOURCE_DESC desc{}; desc.Dimension=D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                desc.Width=m_Clip.Width; desc.Height=m_Clip.Height; desc.DepthOrArraySize=1; desc.MipLevels=1;
                desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM; desc.SampleDesc.Count=1; desc.Layout=D3D12_TEXTURE_LAYOUT_UNKNOWN;
                auto heap=Heap(D3D12_HEAP_TYPE_DEFAULT);
                Check(device->CreateCommittedResource(&heap,D3D12_HEAP_FLAG_NONE,&desc,D3D12_RESOURCE_STATE_COPY_DEST,nullptr,IID_PPV_ARGS(&m_Texture)),"Cannot allocate GIF texture.");
                UINT rows=0; UINT64 rowSize=0;
                device->GetCopyableFootprints(&desc,0,1,0,&m_Footprint,&rows,&rowSize,&m_UploadBytes);
                m_Uploads.resize(bufferCount);
                D3D12_SHADER_RESOURCE_VIEW_DESC srv{}; srv.Format=desc.Format; srv.ViewDimension=D3D12_SRV_DIMENSION_TEXTURE2D;
                srv.Shader4ComponentMapping=D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; srv.Texture2D.MipLevels=1;
                device->CreateShaderResourceView(m_Texture.Get(),&srv,cpu); m_Handle=gpu;
            }
            const auto index=Animation::FrameAt(m_Clip,m_ElapsedMs);
            if (index==m_LastFrame) return true;
            auto& upload=m_Uploads[frameIndex];
            if (!upload)
            {
                D3D12_RESOURCE_DESC desc{}; desc.Dimension=D3D12_RESOURCE_DIMENSION_BUFFER; desc.Width=m_UploadBytes;
                desc.Height=1; desc.DepthOrArraySize=1; desc.MipLevels=1; desc.SampleDesc.Count=1; desc.Layout=D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                auto heap=Heap(D3D12_HEAP_TYPE_UPLOAD);
                Check(device->CreateCommittedResource(&heap,D3D12_HEAP_FLAG_NONE,&desc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&upload)),"Cannot allocate GIF staging buffer.");
            }
            void* data=nullptr; D3D12_RANGE readRange{0,0}; Check(upload->Map(0,&readRange,&data),"Cannot map GIF staging buffer.");
            auto* bytes=static_cast<std::uint8_t*>(data)+m_Footprint.Offset;
            const auto& pixels=m_Clip.Frames[index].Pixels;
            for (unsigned y=0;y<m_Clip.Height;++y)
                std::memcpy(bytes+std::size_t(y)*m_Footprint.Footprint.RowPitch,pixels.data()+std::size_t(y)*m_Clip.Width*4,m_Clip.Width*4);
            D3D12_RANGE writeRange{0,static_cast<SIZE_T>(m_UploadBytes)}; upload->Unmap(0,&writeRange);
            D3D12_RESOURCE_BARRIER barrier{}; barrier.Type=D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource=m_Texture.Get(); barrier.Transition.Subresource=D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            if (m_InShaderState)
            {
                barrier.Transition.StateBefore=D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                barrier.Transition.StateAfter=D3D12_RESOURCE_STATE_COPY_DEST; commands->ResourceBarrier(1,&barrier);
            }
            D3D12_TEXTURE_COPY_LOCATION dst{}; dst.pResource=m_Texture.Get(); dst.Type=D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_TEXTURE_COPY_LOCATION src{}; src.pResource=upload.Get(); src.Type=D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; src.PlacedFootprint=m_Footprint;
            commands->CopyTextureRegion(&dst,0,0,0,&src,nullptr);
            barrier.Transition.StateBefore=D3D12_RESOURCE_STATE_COPY_DEST; barrier.Transition.StateAfter=D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            commands->ResourceBarrier(1,&barrier); m_InShaderState=true; m_LastFrame=index; return true;
        }
        catch (const std::exception& e)
        {
            // No commands are recorded until all allocations/maps succeed.
            m_Status=e.what(); m_FailedGpu=true; m_Handle={}; m_Clip={};
            m_InShaderState=false; m_LastFrame=std::numeric_limits<std::size_t>::max(); return false;
        }
    }
    bool AnimatedBackground::Draw(ImDrawList* draw,ImVec2 pos,ImVec2 size,float opacity,float rounding) const
    {
        if (!m_Texture || !m_InShaderState || !m_Handle.ptr || size.x<=0 || size.y<=0 || opacity<=0) return false;
        const float aspect=float(m_Clip.Width)/m_Clip.Height, target=size.x/size.y;
        ImVec2 uv0(0,0),uv1(1,1);
        if (aspect>target) { const float width=target/aspect; uv0.x=(1-width)*0.5f; uv1.x=uv0.x+width; }
        else { const float height=aspect/target; uv0.y=(1-height)*0.5f; uv1.y=uv0.y+height; }
        draw->AddImageRounded(static_cast<ImTextureID>(m_Handle.ptr),pos,ImVec2(pos.x+size.x,pos.y+size.y),uv0,uv1,
            ImGui::GetColorU32(ImVec4(1,1,1,std::clamp(opacity,0.0f,1.0f))),rounding);
        return true;
    }
}
