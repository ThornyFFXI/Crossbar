#ifndef __ASHITA_CrossbarEditPalette_H_INCLUDED__
#define __ASHITA_CrossbarEditPalette_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarConfirmRemoval.h"

class CrossbarEditPalette : public FontMenuBase
{
private:
    SinglePaletteInfo_t* pPalette;
    CrossbarBindings* pBindings;

public:
    CrossbarEditPalette(FontMenuBase* pMainMenu, CrossbarBindings* pBindings, SinglePaletteInfo_t* pPalette);

private:
    void HandleConfirm() override;
    void HandleSubMenu(FontMenuCompletionData_t data) override;
    void ReceiveText(const char* value) override;

    static FontMenuState GetInitialState(SinglePaletteInfo_t* pBindings);
};

#endif