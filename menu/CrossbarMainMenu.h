#ifndef __ASHITA_CrossbarMainMenu_H_INCLUDED__
#define __ASHITA_CrossbarMainMenu_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarAddPalette.h"
#include "CrossbarManagePalettes.h"
#include "CrossbarBindButton.h"

class CrossbarMainMenu : public FontMenuBase
{
private:
    CrossbarBindings* pBindings;

public:
    CrossbarMainMenu(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings);
    ~CrossbarMainMenu();
    
private:
    static FontMenuState GetInitialState();
    void HandleConfirm() override;
    void HandleSubMenu(FontMenuCompletionData_t data) override;
};

#endif