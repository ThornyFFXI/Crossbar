#ifndef __ASHITA_CrossbarSelectBindFlags_H_INCLUDED__
#define __ASHITA_CrossbarSelectBindFlags_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"

class CrossbarSelectBindFlags : public FontMenuBase
{
private:
    SingleMacroInfo_t mMacro;
    char mButtonName[256];

public:
    CrossbarSelectBindFlags(FontMenuBase* pMainMenu, SingleMacroInfo_t macro, const char* buttonName);


private:
    void ApplySettings();
    DrawSetting GetDrawSetting(const char* text);
    void HandleConfirm() override;
    void HandleButtonUp() override;
    static FontMenuState GetInitialState(SingleMacroInfo_t macro, const char* buttonName);
};

#endif