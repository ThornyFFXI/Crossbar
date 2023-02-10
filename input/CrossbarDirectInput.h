#ifndef __ASHITA_CrossbarDirectInput_H_INCLUDED__
#define __ASHITA_CrossbarDirectInput_H_INCLUDED__
#include "InputHandler.h"
#include <list>

//#define NINTENDOSWITCH

class CrossbarDirectInput
{
private:
    IAshitaCore* pAshitaCore;
    InputHandler* pInput;
    bool mHookActive;
    InputData_t m_ControllerState;

    #ifdef NINTENDOSWITCH
    std::list<uint32_t> mAlwaysBlockOffsets =
    {
        offsetof(DIJOYSTATE, rgbButtons) + 6,
        offsetof(DIJOYSTATE, rgbButtons) + 7,
    };
    #else
    std::list<uint32_t> mAlwaysBlockOffsets = {
            12, //L2 Axis
            16, //R2 Axis
            offsetof(DIJOYSTATE, rgbButtons) + 6,
            offsetof(DIJOYSTATE, rgbButtons) + 7};
    #endif
    std::list<uint32_t> mMacroBlockOffsets = {
        offsetof(DIJOYSTATE, rgbButtons) + 0,
        offsetof(DIJOYSTATE, rgbButtons) + 1,
        offsetof(DIJOYSTATE, rgbButtons) + 2,
        offsetof(DIJOYSTATE, rgbButtons) + 3,
        offsetof(DIJOYSTATE, rgbButtons) + 4,
        offsetof(DIJOYSTATE, rgbButtons) + 5,
        offsetof(DIJOYSTATE, rgdwPOV)};

public:
    CrossbarDirectInput(InputHandler* pInput, IAshitaCore* pAshitaCore);
    ~CrossbarDirectInput();
    bool GetHookActive();
    BOOL ControllerCallback(uint32_t* offset, int32_t* state, bool blocked, bool injected);
    HRESULT GetDeviceState(DWORD cbData, LPVOID lpvData);
};

#endif