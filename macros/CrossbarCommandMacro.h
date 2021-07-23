#ifndef __ASHITA_CrossbarCommandMacro_H_INCLUDED__
#define __ASHITA_CrossbarCommandMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"

class CrossbarCommandMacro : public CrossbarMacro
{
private:
    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsTriggered;

public:
    CrossbarCommandMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarCommandMacro();

    bool Draw(GdiDIB* pDIB) override;
    void TriggerMacro() override;
};

#endif