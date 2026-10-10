#pragma once
#include "GifAnimation.hpp"
#include <d3d12.h>
#include <wrl/client.h>
#include <imgui.h>
#include <chrono>
#include <filesystem>
#include <limits>
#include <string>

namespace YimMenu
{
    class AnimatedBackground
    {
        template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
        Animation::Clip m_Clip;
        ComPtr<ID3D12Resource> m_Texture;
        std::vector<ComPtr<ID3D12Resource>> m_Uploads;
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_Footprint{};
        UINT64 m_UploadBytes = 0;
        D3D12_GPU_DESCRIPTOR_HANDLE m_Handle{};
        bool m_InShaderState = false;
        bool m_FailedGpu = false;
        std::size_t m_LastFrame = std::numeric_limits<std::size_t>::max();
        double m_ElapsedMs = 0;
        std::chrono::steady_clock::time_point m_LastTick = std::chrono::steady_clock::now();
        std::string m_Status = "No animated background loaded.";
    public:
        // Caller must fence/wait outstanding draws before replacing/releasing GPU resources.
        void Reset();
        bool Load(const std::filesystem::path& file);
        void Tick(bool playing, float speed);
        bool Prepare(ID3D12Device* device, ID3D12GraphicsCommandList* commands,
            unsigned frameIndex, unsigned bufferCount,
            D3D12_CPU_DESCRIPTOR_HANDLE cpu, D3D12_GPU_DESCRIPTOR_HANDLE gpu);
        bool Draw(ImDrawList* draw, ImVec2 position, ImVec2 size, float opacity, float rounding) const;
        const std::string& Status() const { return m_Status; }
    };
}
