#ifndef __ASHITA_CrossbarAddPalette_H_INCLUDED__
#define __ASHITA_CrossbarAddPalette_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"

class CrossbarAddPalette : public FontMenuBase
{
private:
    CrossbarBindings* pBindings;

public:
    CrossbarAddPalette(FontMenuBase* pMainMenu, CrossbarBindings* pBindings);
    ~CrossbarAddPalette();

private:
    void HandleConfirm() override;
    static FontMenuState GetInitialState(CrossbarBindings* pBindings);
};

#endif