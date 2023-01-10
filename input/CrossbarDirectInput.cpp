#include "CrossbarDirectInput.h"

CrossbarDirectInput* gpDirectInput = nullptr;

HRESULT __stdcall Mine_GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	return gpDirectInput->GetDeviceState(cbData, lpvData);
}
HRESULT __stdcall Mine_GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, DWORD count, LPDWORD pdwInOut, DWORD dwFlags)
{
	return gpDirectInput->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
}

CrossbarDirectInput::CrossbarDirectInput(InputHandler* pInput, IAshitaCore* pAshitaCore)
	: pAshitaCore(pAshitaCore)
	, pInput(pInput)
{
	gpDirectInput = this;
	mHookActive = false;
	mTriggers[0] = false;
	mTriggers[1] = false;
    pAshitaCore->GetInputManager()->GetController()->AddCallback("Crossbar_Plugin_CB", &Mine_GetDeviceData, &Mine_GetDeviceState, nullptr);
}
CrossbarDirectInput::~CrossbarDirectInput()
{
    pAshitaCore->GetInputManager()->GetController()->RemoveCallback("Crossbar_Plugin_CB");
}

bool CrossbarDirectInput::GetHookActive()
{
    return mHookActive;
}

HRESULT CrossbarDirectInput::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
    if (!mHookActive)
    {
        mHookActive = true;
        pAshitaCore->GetChatManager()->Writef(0, false, "%s%s", Ashita::Chat::Header("Crossbar").c_str(), Ashita::Chat::Message("DirectInput detected!").c_str());
    }

    if (!pInput->GetGameMenuActive())
    {
        const auto joy = (DIJOYSTATE*)lpvData;
        HandleState(joy);
        UpdateState(cbData, lpvData);
    }

	return DI_OK;
}
HRESULT CrossbarDirectInput::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
    if (!mHookActive)
    {
        mHookActive = true;
        pAshitaCore->GetChatManager()->Writef(0, false, "%s%s", Ashita::Chat::Header("Crossbar").c_str(), Ashita::Chat::Message("DirectInput detected!").c_str());
    }

	if (pInput->GetGameMenuActive())
    {
        UpdateData(cbObjectData, rgdod, pdwInOut, dwFlags);
    }
    return DI_OK;
}

void CrossbarDirectInput::HandleState(DIJOYSTATE* pState)
{
	InputData_t data;
	data.LeftTrigger = (pState->rgbButtons[6] & 0x80);
	data.RightTrigger = (pState->rgbButtons[7] & 0x80);
	data.Buttons[0] = (pState->rgbButtons[3] & 0x80);
	data.Buttons[1] = (pState->rgbButtons[2] & 0x80);
	data.Buttons[2] = (pState->rgbButtons[1] & 0x80);
	data.Buttons[3] = (pState->rgbButtons[0] & 0x80);
	data.LeftShoulder = (pState->rgbButtons[4] & 0x80);
	data.RightShoulder = (pState->rgbButtons[5] & 0x80);

	switch (pState->rgdwPOV[0])
	{
	case 0:
		data.Dpad[0] = true;
		break;
	case 9000:
		data.Dpad[1] = true;
		break;
	case 18000:
		data.Dpad[2] = true;
		break;
	case 27000:
		data.Dpad[3] = true;
		break;
	}

	mTriggers[0] = data.LeftTrigger;
	mTriggers[1] = data.RightTrigger;
	pInput->HandleState(data);
}

void CrossbarDirectInput::UpdateData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	if (pInput->GetMenuActive())
	{
		std::memset(rgdod, 0x00, cbObjectData * (*pdwInOut));
		*pdwInOut = 0;
		return;
	}
	
	int numberOfElements = *pdwInOut;
	int element = 0;
	while (element < numberOfElements)
	{
		DIDEVICEOBJECTDATA pData = rgdod[element];
		if (pData.dwOfs == offsetof(DIJOYSTATE, rgbButtons) + 6)
			mTriggers[0] = (pData.dwData & 0x80);
		if (pData.dwOfs == offsetof(DIJOYSTATE, rgbButtons) + 7)
			mTriggers[1] = (pData.dwData & 0x80);

		bool autoBlock = (std::find(mAlwaysBlockOffsets.begin(), mAlwaysBlockOffsets.end(), pData.dwOfs) != mAlwaysBlockOffsets.end());

		if (autoBlock || ((mTriggers[0] || mTriggers[1]) && (std::find(mMacroBlockOffsets.begin(), mMacroBlockOffsets.end(), pData.dwOfs) != mMacroBlockOffsets.end())))
		{
			//Shift future elements back.
			for (int x = (element + 1); x < numberOfElements; x++)
			{
				memcpy(rgdod + (x - 1), rgdod + x, cbObjectData);
			}

			//Adjust count 
			numberOfElements--;
			memset(rgdod + numberOfElements, 0, cbObjectData);
		}
		else
		{
			//only increment iterator if not removing element, if removing the same index will point to new entry
			element++;
		}
	}
	*pdwInOut = numberOfElements;
}
void CrossbarDirectInput::UpdateState(DWORD cbData, LPVOID lpvData)
{
	const auto joy = (DIJOYSTATE*)lpvData;
	if (mTriggers[0] || mTriggers[1] || pInput->GetMenuActive())
	{
		for (int x = 0; x < 4; x++)
		{
			joy->rgdwPOV[x] = (DWORD)-1;
			joy->rgbButtons[x] = 0;
		}
		joy->rgbButtons[4] = 0;
		joy->rgbButtons[5] = 0;
	}
	joy->rgbButtons[6] = 0;
	joy->rgbButtons[7] = 0;
}