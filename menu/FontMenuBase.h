#ifndef __ASHITA_FontMenuBase_H_INCLUDED__
#define __ASHITA_FontMenuBase_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "FontMenuDefines.h"
#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarSettings.h"
#include "FontMenuUI.h"

class FontMenuBase
{
public:
    FontMenuState mState;
    bool mIsSubMenu;
    FontMenuBase* pSubMenu;
    FontMenuCompletionData_t mCompletionData;
    bool mIsFinished;

    IAshitaCore* pAshitaCore;
    CrossbarSettings* pSettings;
    FontMenuUI* pFontUI;

    FontMenuBase(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, FontMenuState state);
    FontMenuBase(FontMenuBase* pBase, FontMenuState state);
    virtual ~FontMenuBase();

    void Draw();
    virtual bool GetIsFinished(FontMenuCompletionData_t* buffer);

    virtual MacroMode GetModeOverride();
    virtual void HandleButtonUp();
    virtual void HandleButtonRight();
    virtual void HandleButtonDown();
    virtual void HandleButtonLeft();
    virtual void HandleConfirm();
    virtual void HandleCancel();

    virtual void HandleDpadUp();
    virtual void HandleDpadRight();
    virtual void HandleDpadDown();
    virtual void HandleDpadLeft();

    virtual void HandleMacro(MacroButton button, MacroMode mode);
    virtual void HandleSubMenu(FontMenuCompletionData_t data);
    virtual void ReceiveText(const char* value);
};

#endif