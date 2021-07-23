#ifndef __ASHITA_CrossbarConfirmRemoval_H_INCLUDED__
#define __ASHITA_CrossbarConfirmRemoval_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"

class CrossbarConfirmRemoval : public FontMenuBase
{
private:
    SinglePaletteInfo_t* pPalette;
    CrossbarBindings* pBindings;

public:
    CrossbarConfirmRemoval(FontMenuBase* pMainMenu, const char* confirmText, const char* returnValue);

private:
    void HandleConfirm() override;
    static FontMenuState GetInitialState(const char* confirmText, const char* returnValue);
};

#endif