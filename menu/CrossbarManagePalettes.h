#ifndef __ASHITA_CrossbarManagePalettes_H_INCLUDED__
#define __ASHITA_CrossbarManagePalettes_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarEditPalette.h"

class CrossbarManagePalettes : public FontMenuBase
{
private:
    CrossbarBindings* pBindings;

public:
    CrossbarManagePalettes(FontMenuBase* pMainMenu, CrossbarBindings* pBindings);

private:
    void HandleConfirm() override;
    void HandleButtonUp() override;
    void HandleButtonLeft() override;
    void HandleSubMenu(FontMenuCompletionData_t data) override;

    static FontMenuState GetInitialState(CrossbarBindings* pBindings);
};

#endif