#ifndef __ASHITA_CrossbarSpellIcon_H_INCLUDED__
#define __ASHITA_CrossbarSpellIcon_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"

class CrossbarSpellMacro : public CrossbarMacro
{
private:
    ISpell* pSpell;
    std::list<int16_t> mRequiredBuffs;

    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsBlocked;
    bool mIsTransparent;
    bool mIsTriggered;
    int mRecast;

public:
    CrossbarSpellMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarSpellMacro();

    bool CheckSpellAvailable();
    bool CheckAddendum(int spell);
    bool Draw(GdiDIB* pDIB) override;
    void TriggerMacro() override;
};

#endif