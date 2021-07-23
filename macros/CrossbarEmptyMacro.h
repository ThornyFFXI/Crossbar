#ifndef __ASHITA_CrossbarEmptyMacro_H_INCLUDED__
#define __ASHITA_CrossbarEmptyMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"

class CrossbarEmptyMacro : public CrossbarMacro
{
public:
    CrossbarEmptyMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarEmptyMacro();

    bool Draw(GdiDIB* pDIB) override;
    void TriggerMacro() override;
};

#endif