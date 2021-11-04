// dllmain.cpp: This is where the magic happens!

/**
 dbzk_fps (C) 2021 Bryce Q.

 dbzk_fps is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 dbzk_fps is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include "../Source/pch.h"
#include "wtypes.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <process.h>
#include <stdio.h>
// Silent's ModUtils for unprotecting the main module, pattern scanning, and inserting trampolines.
#include "../Source/ThirdParty/ModUtils/MemoryMgr.h"
#include "../Source/ThirdParty/ModUtils/Patterns.h"
#include "../Source/ThirdParty/ModUtils/Trampoline.h"

using namespace std;
using namespace hook;
using namespace Memory::VP;

// Misc variables
bool check = true; // do not change to false or else resolution checks won't run.

float maxFPS = 9999.00f;

// Process HMODULE variable
HMODULE baseModule = GetModuleHandle(NULL);

namespace framerateUncap
{
	void uncapFPS() //Uncaps the framerate.
	{
		try
		{
			// We only want to detour the function on the first five bytes, which is the opcode that accesses the register containing the framerate cap.
			auto fpsCapFunc_pattern = pattern("f3 0f 10 04 83 0f 28 74 24 ? 44 0f 28 4c 24 ? 48 83 c4 ? 5b c3 b0");
			// Afterwards, we then jump back to the part after the opcode is written to.
			auto fpsCapFuncJump_pattern = pattern("0f 28 74 24 ? 44 0f 28 4c 24 ? 48 83 c4 ? 5b c3 b0");

			if (fpsCapFunc_pattern.count(1).size() == 1 && fpsCapFuncJump_pattern.count(1).size() == 1)
			{
				auto fpsCapFunc = fpsCapFunc_pattern.get_first();
				auto fpsCapFuncJump = fpsCapFuncJump_pattern.get_first();

				Trampoline* trampoline = Trampoline::MakeTrampoline(fpsCapFunc);

				float* ptr = trampoline->Pointer<float>();
				*ptr = maxFPS;

				// Creates a space that we can use to make assembly functions using binary representations of them.
				const uint8_t fpsCapPayload[] = {
					//0xf3, 0x0f, 0x10, 0x04, 0x83, // movss xmm0,[rbx+rax*4] (original FPS cap instruction)
					0xF3, 0x0F, 0x10, 0x05, 0x00, 0x00, 0x00, 0x00, // movss xmm0,[ptr] (We need to change the 04 byte to 05 for some reason before doing this)
					0xE9, 0x00, 0x00, 0x00, 0x00, // jmp fpsCapFuncJump+6 (Add a byte to the count of the original FPS cap instruction to go to the next instruction)
				};

				std::byte* space = trampoline->RawSpace(sizeof(fpsCapPayload)); 
				memcpy(space, fpsCapPayload, sizeof(fpsCapPayload)); // Creates a trampoline the size of our FPS cap payload.

				WriteOffsetValue(space + 4, ptr); // Start the offset for the pointer reference on the fifth byte of the new FPS cap instruction
				WriteOffsetValue(space + 8 + 1, reinterpret_cast<intptr_t>(fpsCapFuncJump)); // Place the JMP address on the tenth byte in our code cave.

				// Inject a hook into the function and redirect it to the ASM function that we want to modify.
				InjectHook(fpsCapFunc, space, PATCH_JUMP);
			}
		}
		TXN_CATCH();
	}
}

void StartPatch()
{
	// Unprotects the main module handle.
    ScopedUnprotect::FullModule UnProtect(baseModule);

	framerateUncap::uncapFPS();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) // This code runs when the DLL file has been attached to the game process.
    {
		HANDLE patchThread; // Creates a handle to the patch thread, so it can be closed easier
		patchThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall*)(void*))StartPatch, NULL, 0, 0); // Calls the StartPatch function in a new thread on start.
		// CloseHandle(patchThread); // Closes the StartPatch thread handle.
    }
    return TRUE;
}
