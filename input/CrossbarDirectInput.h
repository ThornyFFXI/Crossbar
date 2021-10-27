#ifndef __ASHITA_CrossbarDirectInput_H_INCLUDED__
#define __ASHITA_CrossbarDirectInput_H_INCLUDED__
#include "InputHandler.h"
#include "C:\Ashita 4\plugins\sdk\d3d8\includes\dinput.h"
#pragma comment(lib, "C:/Ashita 4/plugins/sdk/d3d8/lib/dinput8.lib")
#pragma comment(lib, "C:/Ashita 4/plugins/sdk/d3d8/lib/dxguid.lib")
#include <list>

#define RBUFP(p,pos) (((uint8_t*)(p)) + (pos))
#define Read32(p,pos) (*(uint32_t*)RBUFP((p),(pos)))

typedef HRESULT (WINAPI* DIGetDeviceState)(IDirectInputDevice8A*, DWORD, LPVOID);
typedef HRESULT (WINAPI* DIGetDeviceData)(IDirectInputDevice8A*, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);

class CrossbarDirectInput
{
private:
    IDirectInput8A* pDirectInput;
    LPDIRECTINPUTDEVICE8 pDirectInputDevice;
    DIGetDeviceState Real_GetDeviceState;
    DIGetDeviceData Real_GetDeviceData;
    bool mHookActive;
    InputHandler* pInput;
    DWORD pvTable;

    std::list<int> mAlwaysBlockOffsets =
    {
        12, //L2 Axis
        16, //R2 Axis
        offsetof(DIJOYSTATE, rgbButtons) + 6,
        offsetof(DIJOYSTATE, rgbButtons) + 7
    };

    std::list<int> mMacroBlockOffsets = {
        offsetof(DIJOYSTATE, rgbButtons) + 0,
        offsetof(DIJOYSTATE, rgbButtons) + 1,
        offsetof(DIJOYSTATE, rgbButtons) + 2,
        offsetof(DIJOYSTATE, rgbButtons) + 3,
        offsetof(DIJOYSTATE, rgbButtons) + 4,
        offsetof(DIJOYSTATE, rgbButtons) + 5,
        offsetof(DIJOYSTATE, rgdwPOV)
    };

    bool mTriggers[2];

public:
    CrossbarDirectInput(InputHandler* pInput);
    ~CrossbarDirectInput();
    bool AttemptHook();
    bool GetHookActive();
    HRESULT GetDeviceState(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData);
    HRESULT GetDeviceData(IDirectInputDevice8A* pDevice, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);

    void HandleFoundDevice(GUID guid);
    void HandleState(DIJOYSTATE* pState);
    void HandleStateExtended(DIJOYSTATE2* pState);
    void UpdateData(IDirectInputDevice8A* pDevice, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    void UpdateState(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData);
    void UpdateStateExtended(IDirectInputDevice8A* pDevice, DWORD cbData, LPVOID lpvData);
};

#endif