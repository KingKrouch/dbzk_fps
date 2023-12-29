/**
dbzk_fps Copyright (c) 2023 Bryce Q.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#include "PatchManager.h"
#include "../Settings/PlayerSettings.h"
#include "../ThirdParty/ModUtils/MemoryMgr.h"
#include "../ThirdParty/ModUtils/ScopedUnprotect.hpp"
#include "../ThirdParty/ModUtils/Patterns.h"
#include "../ThirdParty/ModUtils/Trampoline.h"

auto& PlayerSettingsPm = dbzk_fps::PlayerSettings::Get();

// Singleton Instance
dbzk_fps::PatchManager dbzk_fps::PatchManager::pm_Instance;

// Process HMODULE variable
HMODULE BaseModule = GetModuleHandle(NULL);

bool Loop = true; // do not change to false or else resolution checks won't run.

using namespace std;
using namespace hook;
using namespace Memory::VP;

DWORD64 returnJmp;
//void __declspec(naked) FramerateCap_CC() {
    //asm volatile (
            //"movss %%xmm0, %[maxFPS];"
            //"jmp *%[returnJmp];"
            //:
            //: [maxFPS] "m" (PlayerSettingsPm.SYNC.MaxFPS), [returnJmp] "r" (returnJmp)
    //: "xmm0"  // Clobbered registers
   //);
//}

namespace dbzk_fps {
    void PatchManager::InitPatch() { // Instead of running our game specific patches in InitPatch(), we will do that in RunPatches(). We just want to initialize the desired game mode and do any preliminary work here.
        ScopedUnprotect::FullModule UnProtect(BaseModule);
    }

    void PatchManager::UncapFPS() {
        *(float*)(*((intptr_t*)((intptr_t)BaseModule + 0x0552A4A8)) + 0x0) = (float)PlayerSettingsPm.SYNC.MaxFPS;

        // We only want to detour the function on the first five bytes, which is the opcode that accesses the register containing the framerate cap.
        //const char* fpsCapFunc_pattern = "f3 0f 10 04 83 0f 28 74 24 ? 44 0f 28 4c 24 ? 48 83 c4 ? 5b c3 b0";
        // Afterward, we then jump back to the part after the opcode is written to.
        //const char* fpsCapFuncJump_pattern = "0f 28 74 24 ? 44 0f 28 4c 24 ? 48 83 c4 ? 5b c3 b0";
        // Make Patterns for our const char* variables.
        //auto fpsCapFuncPat = pattern(fpsCapFunc_pattern);
        //auto fpsCapFuncJumpPat = pattern(fpsCapFuncJump_pattern);

        //if (fpsCapFuncPat.count(1).size() == 1 && fpsCapFuncJumpPat.count(1).size() == 1) {
            //auto fpsCapFunc = fpsCapFuncPat.get_first();
            //auto fpsCapFuncJump = fpsCapFuncJumpPat.get_first();
            //auto tramp = Trampoline::MakeTrampoline(fpsCapFunc);
            //if (tramp) {
                //returnJmp = reinterpret_cast<DWORD64>(fpsCapFuncJump);
                //InjectHook(fpsCapFunc, tramp->Jump(FramerateCap_CC));
            //} else {
                // Handle trampoline creation failure
            //}
        //}
        //else {
            // Handle pattern not found
        //}
    }

    void PatchManager::CheckFPS() {
        if (*(float*)(*((intptr_t*)((intptr_t)BaseModule + 0x0552A4A8)) + 0x0) != (float)PlayerSettingsPm.SYNC.MaxFPS) {
            UncapFPS();
        }
    }
    

    void PatchManager::RunPatches() {
        Sleep(5000);
        UncapFPS();
        while (Loop != false) {
            CheckFPS();
        }
    }
}