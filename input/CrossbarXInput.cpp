#include "CrossbarXInput.h"
#include <intrin.h>
#include <psapi.h>
#include <Windows.h>

CrossbarXInput* gpXInput = nullptr;

DWORD __stdcall Mine_XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	return gpXInput->XInputGetState(dwUserIndex, pState);
}

CrossbarXInput::CrossbarXInput(InputHandler* pInput)
	: pInput(pInput)
{
	gpXInput = this;
	Real_XInputGetState = nullptr;
	mHookActive = false;
}

CrossbarXInput::~CrossbarXInput()
{
	if (mHookActive)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(LPVOID&)Real_XInputGetState, Mine_XInputGetState);
		DetourTransactionCommit();
	}
}

bool CrossbarXInput::AttemptHook()
{
	if (mHookActive)
		return false;

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, GetCurrentProcessId());
	if (NULL == hProcess)
		return 1;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				FARPROC address = GetProcAddress(hMods[i], "XInputGetState");
				if ((strstr(szModName, "XINPUT1_3.dll")) && (address))
				{
					Real_XInputGetState = (XInputGetStateFunc)address;
					::DetourTransactionBegin();
					::DetourUpdateThread(::GetCurrentThread());
					uint32_t result = ::DetourAttach(&(PVOID&)Real_XInputGetState, Mine_XInputGetState);
					uint32_t result2 = ::DetourTransactionCommit();
					if ((result == 0) && (result2 == 0))
					{
						mHookActive = true;
						break;
					}
				}
			}
		}
	}
	CloseHandle(hProcess);
	return mHookActive;
}
DWORD CrossbarXInput::XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	DWORD retValue = Real_XInputGetState(dwUserIndex, pState);
    if (pInput->GetGameMenuActive())
        return retValue;

	HandleState(dwUserIndex, pState);
	UpdateState(dwUserIndex, pState);
	return retValue;
}
void CrossbarXInput::HandleState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	InputData_t data;
	data.LeftTrigger = (pState->Gamepad.bLeftTrigger != 0);
	data.RightTrigger = (pState->Gamepad.bRightTrigger != 0);
	data.Buttons[0] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_Y);
	data.Buttons[1] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_B);
	data.Buttons[2] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_A);
	data.Buttons[3] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_X);
	data.LeftShoulder = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
	data.RightShoulder = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
	data.Dpad[0] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	data.Dpad[1] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	data.Dpad[2] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	data.Dpad[3] = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	pInput->HandleState(data);
}
void CrossbarXInput::UpdateState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	if ((pState->Gamepad.bLeftTrigger) || (pState->Gamepad.bRightTrigger) || (pInput->GetMenuActive()))
	{
		pState->Gamepad.wButtons &= mMacroBlockMask;
	}
	pState->Gamepad.bLeftTrigger = 0;
	pState->Gamepad.bRightTrigger = 0;
}