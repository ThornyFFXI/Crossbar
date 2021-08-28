#include "CrossbarDirectInput.h"

CrossbarDirectInput* gpDirectInput = nullptr;

BOOL CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	gpDirectInput->HandleFoundDevice(lpddi->guidInstance);
	return DIENUM_CONTINUE;
}
HRESULT __stdcall Mine_GetDeviceState(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData)
{
	return gpDirectInput->GetDeviceState(pDevice, cbData, lpvData);
}
HRESULT __stdcall Mine_GetDeviceData(IDirectInputDevice8A* pDevice, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	return gpDirectInput->GetDeviceData(pDevice, cbObjectData, rgdod, pdwInOut, dwFlags);
}

CrossbarDirectInput::CrossbarDirectInput(InputHandler* pInput)
	: pInput(pInput)
{
	gpDirectInput = this;
	Real_GetDeviceState = nullptr;
	mHookActive = false;
	mTriggers[0] = false;
	mTriggers[1] = false;
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirectInput, NULL);
}
CrossbarDirectInput::~CrossbarDirectInput()
{
    if (mHookActive)
    {
        int size = sizeof(intptr_t);
        DWORD oldProtection;
        DWORD tempProtection;

        int offset  = 9;
        DWORD field = pvTable + (offset * sizeof(intptr_t));
        VirtualProtect(reinterpret_cast<void*>(field), size, PAGE_EXECUTE_READWRITE, &oldProtection);
        *(reinterpret_cast<DIGetDeviceState*>(field)) = Real_GetDeviceState;
        VirtualProtect(reinterpret_cast<void*>(field), size, oldProtection, &tempProtection);

        offset = 10;
        field  = pvTable + (offset * sizeof(intptr_t));
        VirtualProtect(reinterpret_cast<void*>(field), size, PAGE_EXECUTE_READWRITE, &oldProtection);
        *(reinterpret_cast<DIGetDeviceData*>(field)) = Real_GetDeviceData;
        VirtualProtect(reinterpret_cast<void*>(field), size, oldProtection, &tempProtection);
    }

    if (pDirectInput)
    {
        pDirectInput->Release();
        pDirectInput = nullptr;
    }
}

bool CrossbarDirectInput::AttemptHook()
{
	if (mHookActive)
		return false;
	
	int nJoysticks = 0;
	pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, &nJoysticks, DIEDFL_ALLDEVICES);
	return mHookActive;
}
HRESULT CrossbarDirectInput::GetDeviceState(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData)
{
	DIDEVICEINSTANCEA info;
	pDevice->GetDeviceInfo(&info);
	HRESULT result = Real_GetDeviceState(pDevice, cbData, lpvData);
	if (info.dwDevType == DI8DEVTYPE_MOUSE || info.dwDevType == DI8DEVTYPE_KEYBOARD)
	{
		return result;
	}

    if (pInput->GetGameMenuActive())
        return result;

	if (sizeof(DIJOYSTATE) == cbData)
	{
		const auto joy = (DIJOYSTATE*)lpvData;
		HandleState(joy);
		UpdateState(pDevice, cbData, lpvData);
	}
	else if (sizeof(DIJOYSTATE2) == cbData)
	{
		const auto joy = (DIJOYSTATE2*)lpvData;
		HandleStateExtended(joy);
		UpdateStateExtended(pDevice, cbData, lpvData);
	}

	return result;
}
HRESULT CrossbarDirectInput::GetDeviceData(IDirectInputDevice8A* pDevice, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	DIDEVICEINSTANCEA info;
	pDevice->GetDeviceInfo(&info);
	HRESULT result = Real_GetDeviceData(pDevice, cbObjectData, rgdod, pdwInOut, dwFlags);
	if (info.dwDevType == DI8DEVTYPE_MOUSE || info.dwDevType == DI8DEVTYPE_KEYBOARD)
	{
		return result;
	}

	if (pInput->GetGameMenuActive())
        return result;

	UpdateData(pDevice, cbObjectData, rgdod, pdwInOut, dwFlags);
	return result;
}

void CrossbarDirectInput::HandleFoundDevice(GUID guid)
{
	if (!mHookActive)
	{
		HRESULT result = pDirectInput->CreateDevice(guid, &pDirectInputDevice, NULL);
		if (result == 0)
		{
			pvTable = Read32(Read32(&pDirectInputDevice, 0), 0);
			int size = sizeof(intptr_t);
			DWORD oldProtection;
			DWORD tempProtection;
			int offset = 9;
			DWORD field = pvTable + (offset * sizeof(intptr_t));
			VirtualProtect(reinterpret_cast<void*>(field), size, PAGE_EXECUTE_READWRITE, &oldProtection);
			Real_GetDeviceState = *(reinterpret_cast<DIGetDeviceState*>(field));
			*(reinterpret_cast<DIGetDeviceState*>(field)) = Mine_GetDeviceState;
			VirtualProtect(reinterpret_cast<void*>(field), size, oldProtection, &tempProtection);

			offset = 10;
			field = pvTable + (offset * sizeof(intptr_t));
			VirtualProtect(reinterpret_cast<void*>(field), size, PAGE_EXECUTE_READWRITE, &oldProtection);
			Real_GetDeviceData = *(reinterpret_cast<DIGetDeviceData*>(field));
			*(reinterpret_cast<DIGetDeviceData*>(field)) = Mine_GetDeviceData;
			VirtualProtect(reinterpret_cast<void*>(field), size, oldProtection, &tempProtection);

			mHookActive = true;
			if (pDirectInputDevice)
			{
                pDirectInputDevice->Release();
			}
		}
	}
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
void CrossbarDirectInput::HandleStateExtended(DIJOYSTATE2* pState)
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
void CrossbarDirectInput::UpdateData(IDirectInputDevice8A* pDevice, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
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
void CrossbarDirectInput::UpdateState(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData)
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
void CrossbarDirectInput::UpdateStateExtended(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData)
{
	const auto joy = (DIJOYSTATE2*)lpvData;
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
