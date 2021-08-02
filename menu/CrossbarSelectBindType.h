#ifndef __ASHITA_CrossbarSelectBindType_H_INCLUDED__
#define __ASHITA_CrossbarSelectBindType_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarSelectBindAction.h"
#include <map>

class CrossbarSelectBindType : public FontMenuBase
{
private:
    SingleMacroInfo_t* pButton;
    char mButtonName[256];
    MacroMode mMode;

public:
    CrossbarSelectBindType(FontMenuBase* pMainMenu, SingleMacroInfo_t* ppButton, const char* buttonName, MacroMode mode);

private:
    MacroMode GetModeOverride() override;
    void HandleConfirm() override;
    void HandleButtonUp() override;
    void HandleSubMenu(FontMenuCompletionData_t data) override;

    static FontMenuState GetInitialState(IAshitaCore* pAshitaCore, const char* button, bool useSync);
};

#endif