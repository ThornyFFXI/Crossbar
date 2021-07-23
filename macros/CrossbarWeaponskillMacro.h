#ifndef __ASHITA_CrossbarWeaponskillMacro_H_INCLUDED__
#define __ASHITA_CrossbarWeaponskillMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"
#include "..\skillchain\ResonationTracker.h"

class CrossbarWeaponskillMacro : public CrossbarMacro
{
private:
    IAbility* pAbility;

    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsBlocked;
    bool mIsTransparent;
    bool mIsTriggered;
    bool mIconSwapped;
    int mSkillchainStep;
    int mSkillchainProgress;
    std::chrono::steady_clock::time_point mFrameAdvance;

public:
    static ResonationTracker* pResonation;

    CrossbarWeaponskillMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarWeaponskillMacro();

    bool Draw(GdiDIB* pDIB) override;
    void TriggerMacro() override;
};

#endif