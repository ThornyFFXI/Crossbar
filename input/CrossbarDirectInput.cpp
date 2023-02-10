#include "CrossbarDirectInput.h"

CrossbarDirectInput* gpDirectInput = nullptr;

BOOL __stdcall gControllerCallback(uint32_t* offset, int32_t* state, bool blocked, bool injected)
{
    return gpDirectInput->ControllerCallback(offset, state, blocked, injected);
}

HRESULT __stdcall Mine_GetDeviceState(DWORD cbData, LPVOID lpvData)
{
    return gpDirectInput->GetDeviceState(cbData, lpvData);
}

CrossbarDirectInput::CrossbarDirectInput(InputHandler* pInput, IAshitaCore* pAshitaCore)
	: pAshitaCore(pAshitaCore)
	, pInput(pInput)
    , m_ControllerState(InputData_t())
{
	gpDirectInput = this;
	mHookActive = false;
    pAshitaCore->GetInputManager()->GetController()->AddCallback("Crossbar_Plugin_CB", nullptr, &Mine_GetDeviceState, &gControllerCallback);
}

CrossbarDirectInput::~CrossbarDirectInput()
{
    pAshitaCore->GetInputManager()->GetController()->RemoveCallback("Crossbar_Plugin_CB");
}

bool CrossbarDirectInput::GetHookActive()
{
    return mHookActive;
}

BOOL CrossbarDirectInput::ControllerCallback(uint32_t* offset, int32_t* state, bool blocked, bool injected)
{
    #ifdef NINTENDOSWITCH
    switch (*offset)
    {
        case 32:
            m_ControllerState.Dpad[0] = false;
            m_ControllerState.Dpad[1] = false;
            m_ControllerState.Dpad[2] = false;
            m_ControllerState.Dpad[3] = false;
            switch (*state)
            {
                case 0:
                    m_ControllerState.Dpad[0] = true;
                    break;
                case 9000:
                    m_ControllerState.Dpad[1] = true;
                    break;
                case 18000:
                    m_ControllerState.Dpad[2] = true;
                    break;
                case 27000:
                    m_ControllerState.Dpad[3] = true;
                    break;
            }
            break;
        case 48:
            m_ControllerState.Buttons[2] = (*state & 0x80);
            break;
        case 49:
            m_ControllerState.Buttons[1] = (*state & 0x80);
            break;
        case 50:
            m_ControllerState.Buttons[3] = (*state & 0x80);
            break;
        case 51:
            m_ControllerState.Buttons[0] = (*state & 0x80);
            break;
        case 52:
            m_ControllerState.LeftShoulder = (*state & 0x80);
            break;
        case 53:
            m_ControllerState.RightShoulder = (*state & 0x80);
            break;
        case 54:
            m_ControllerState.LeftTrigger = (*state & 0x80);
            break;
        case 55:
            m_ControllerState.RightTrigger = (*state & 0x80);
            break;
    }
    #else
	switch (*offset)
	{
        case 32:
            m_ControllerState.Dpad[0] = false;
            m_ControllerState.Dpad[1] = false;
            m_ControllerState.Dpad[2] = false;
            m_ControllerState.Dpad[3] = false;
            switch (*state)
            {
                case 0:
                    m_ControllerState.Dpad[0] = true;
                    break;
                case 9000:
                    m_ControllerState.Dpad[1] = true;
                    break;
                case 18000:
                    m_ControllerState.Dpad[2] = true;
                    break;
                case 27000:
                    m_ControllerState.Dpad[3] = true;
                    break;
            }
            break;
        case 48:
            m_ControllerState.Buttons[3] = (*state & 0x80);
            break;
        case 49:
            m_ControllerState.Buttons[2] = (*state & 0x80);
            break;
        case 50:
            m_ControllerState.Buttons[1] = (*state & 0x80);
            break;
        case 51:
            m_ControllerState.Buttons[0] = (*state & 0x80);
            break;
        case 52:
            m_ControllerState.LeftShoulder = (*state & 0x80);
            break;
        case 53:
            m_ControllerState.RightShoulder = (*state & 0x80);
            break;
        case 54:
            m_ControllerState.LeftTrigger = (*state & 0x80);
            break;
        case 55:
            m_ControllerState.RightTrigger = (*state & 0x80);
            break;
	}
    #endif

    if (std::find(mAlwaysBlockOffsets.begin(), mAlwaysBlockOffsets.end(), *offset) != mAlwaysBlockOffsets.end())
        return true;

    if ((m_ControllerState.LeftTrigger) || (m_ControllerState.RightTrigger) || (pInput->GetMenuActive()))
        return (std::find(mMacroBlockOffsets.begin(), mMacroBlockOffsets.end(), *offset) != mMacroBlockOffsets.end());

    return false;
}

HRESULT CrossbarDirectInput::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
    if (!mHookActive)
    {
        mHookActive = true;
        pAshitaCore->GetChatManager()->Writef(0, false, "%s%s", Ashita::Chat::Header("Crossbar").c_str(), Ashita::Chat::Message("DirectInput detected!").c_str());
    }

    pInput->HandleState(m_ControllerState);

	return DI_OK;
}