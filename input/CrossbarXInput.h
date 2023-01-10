#ifndef __ASHITA_CrossbarXInput_H_INCLUDED__
#define __ASHITA_CrossbarXInput_H_INCLUDED__
#include "InputHandler.h"
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
#include "../thirdparty/detours/detours.h"
#pragma comment(lib, "thirdparty/detours/detours.lib")

class CrossbarXInput
{
private:
    IAshitaCore* pAshitaCore;
    bool mHookActive;
    InputHandler* pInput;
    bool mTriggers[2];
    WORD mMacroBlockMask = ~(XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_Y | XINPUT_GAMEPAD_B | XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_LEFT_SHOULDER | XINPUT_GAMEPAD_RIGHT_SHOULDER);

public:
    CrossbarXInput(InputHandler* pInput, IAshitaCore* pAshitaCore);
    ~CrossbarXInput();
    bool GetHookActive();
    DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
    void HandleState(DWORD dwUserIndex, XINPUT_STATE* pState);
    void UpdateState(DWORD dwUserIndex, XINPUT_STATE* pState);
};

#endif