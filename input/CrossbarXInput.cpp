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
    mHookActive         = false;
    pAshitaCore->GetInputManager()->GetXInput()->AddCallback("Crossbar_Plugin_CB", &Mine_XInputGetState, nullptr);
}

CrossbarXInput::~CrossbarXInput()
{
    pAshitaCore->GetInputManager()->GetXInput()->RemoveCallback("Crossbar_Plugin_CB");
}

bool CrossbarXInput::GetHookActive()
{
    return mHookActive;
}

DWORD CrossbarXInput::XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
    if (!mHookActive)
    {
        mHookActive = true;
        pAshitaCore->GetChatManager()->Writef(0, false, "%s%s", Ashita::Chat::Header("Crossbar").c_str(), Ashita::Chat::Message("XInput detected!").c_str());
    }

    if (!pInput->GetGameMenuActive())
    {
        HandleState(dwUserIndex, pState);
        UpdateState(dwUserIndex, pState);
    }

	return ERROR_SUCCESS;
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