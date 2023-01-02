#include "CrossbarXInput.h"
#include <intrin.h>
#include <psapi.h>
#include <Windows.h>

CrossbarXInput* gpXInput = nullptr;

DWORD __stdcall Mine_XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	return gpXInput->XInputGetState(dwUserIndex, pState);
}

CrossbarXInput::CrossbarXInput(InputHandler* pInput, IAshitaCore* pAshitaCore)
	: pInput(pInput)
    , pAshitaCore(pAshitaCore)
{
	gpXInput = this;
	Real_XInputGetState = nullptr;
    mHookActive         = false;
    mRateLimit          = std::chrono::steady_clock::now();
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

bool CrossbarXInput::GetHookActive()
{
    return mHookActive;
}
bool CrossbarXInput::AttemptHook()
{
    if ((mHookActive) || (std::chrono::steady_clock::now() < mRateLimit))
        return false;

    auto handle = ::GetModuleHandleA("xinput1_3.dll");
    if (handle == nullptr)
    {
        handle = ::LoadLibraryA("xinput1_3.dll");
    }
    if (handle != nullptr)
    {
        const auto address = ::GetProcAddress(handle, "XInputGetState");
        if (address != nullptr)
        {

            Real_XInputGetState = reinterpret_cast<XInputGetStateFunc>(address);

            ::DetourTransactionBegin();
            ::DetourUpdateThread(::GetCurrentThread());
            ::DetourAttach(&(PVOID&)Real_XInputGetState, Mine_XInputGetState);
            if (::DetourTransactionCommit() == NO_ERROR)
                mHookActive = true;
        }
    }
    mRateLimit = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    if (!mHookActive)
    {
        pAshitaCore->GetChatManager()->Writef(0, false, "%s%s", Ashita::Chat::Header("Crossbar").c_str(), Ashita::Chat::Error("Failed to hook xinput.  If you are not using an xinput controller, please disable in config and reload crossbar.").c_str());
    }
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