#include "../../src/core/renderer/GifAnimation.hpp"
#include "../../src/game/features/vehicle/SavageCameraPolicy.hpp"
#include <cassert>
#include <iostream>
#include <limits>
int main()
{
 using namespace YimMenu::Animation;
 Clip clip{1,1,{{{255,0,0,255},70},{{0,255,0,255},60},{{0,0,255,255},100}}};
 assert(FrameAt(clip,0)==0 && FrameAt(clip,69)==0 && FrameAt(clip,70)==1);
 assert(FrameAt(clip,130)==2 && FrameAt(clip,230)==0);
 assert(FrameAt(clip,-5)==0 && FrameAt(clip,std::numeric_limits<double>::infinity())==0);
 clip.Frames[0].DelayMs=0; assert(FrameAt(clip,20)==1);
 assert(FrameAt({},50)==0);
 for (const auto& bad:std::vector<std::vector<unsigned>>{{0,1,1},{1025,1,1},{1,1,181},{1024,1024,180}})
 { bool failed=false;try{ValidateDimensions(bad[0],bad[1],bad[2]);}catch(...){failed=true;}assert(failed); }
 Canvas canvas(2,1);
 auto red=canvas.Push(0,0,1,1,{255,0,0,255},1,70); assert(red.Pixels[0]==255 && red.Pixels[7]==0);
 auto green=canvas.Push(1,0,1,1,{0,255,0,255},3,60); assert(green.Pixels[5]==255 && green.Pixels[0]==255);
 auto restored=canvas.Push(0,0,1,1,{0,0,0,0},2,100); assert(restored.Pixels[0]==255 && restored.Pixels[7]==0);
 auto cleared=canvas.Push(1,0,1,1,{0,0,255,255},1,100); assert(cleared.Pixels[3]==0 && cleared.Pixels[6]==255);
 bool bounds=false;try{canvas.Push(2,0,1,1,{0,0,0,255},0,50);}catch(...){bounds=true;}assert(bounds);
 using YimMenu::CameraPolicy::SavageEntryGuard;
 SavageEntryGuard guard;
 assert(!guard.Update(0,false,false,false,false,0));
 assert(!guard.Update(11,true,false,true,false,100));
 assert(guard.Update(11,true,true,true,false,120));
 assert(!guard.Update(11,true,true,true,false,121)); // not a continuous lock
 assert(!guard.Update(0,false,false,false,false,150));
 assert(!guard.Update(12,false,true,true,false,160)); // other vehicles untouched
 assert(!guard.Update(13,true,true,true,true,180)); // manual camera cycle cancels
 assert(!guard.Update(13,true,true,true,false,181));
 guard.Reset();assert(!guard.Update(14,true,true,false,false,200));
 assert(!guard.Update(14,true,true,true,false,1801)); // entry timeout
 guard.Reset();assert(!guard.Update(15,true,false,true,false,0xfffffff0));
 assert(guard.Update(15,true,true,true,false,20)); // unsigned timer wrap
 std::cout<<"PASS: GIF timing/wrap/limits, transparency/subrectangles/disposal 2+3, and Savage one-shot entry/manual-cycle/timeout/timer-wrap policy.\n";
}
