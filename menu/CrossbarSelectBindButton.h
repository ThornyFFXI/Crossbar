#ifndef __ASHITA_CrossbarSelectBindButton_H_INCLUDED__
#define __ASHITA_CrossbarSelectBindButton_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarSelectBindType.h"

class CrossbarSelectBindButton : public FontMenuBase
{
private:
    CrossbarBindings* pBindings;
    SingleBindingInfo_t* pPalette;
    bool mGlobal;

public:
    CrossbarSelectBindButton(FontMenuBase* pMainMenu, CrossbarBindings* pBindings, SingleBindingInfo_t* pPalette, bool global);

private:
    void HandleCancel() override;
    void HandleConfirm() override;
    void HandleMacro(MacroButton button, MacroMode mode) override;
    static FontMenuState GetInitialState(CrossbarBindings* pBindings);

    void HandleSubMenu(FontMenuCompletionData_t data) override;
};

#endif