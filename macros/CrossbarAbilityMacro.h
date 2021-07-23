#ifndef __ASHITA_CrossbarAbilityMacro_H_INCLUDED__
#define __ASHITA_CrossbarAbilityMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"

class CrossbarAbilityMacro : public CrossbarMacro
{
private:
    IAbility* pAbility;
    std::list<int16_t> mRequiredBuffs;

    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsBlocked;
    bool mIsTransparent;
    bool mIsTriggered;
    int mRecast;

public:
    CrossbarAbilityMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarAbilityMacro();

    bool Draw(GdiDIB* pDIB) override;
    void TriggerMacro() override;
};

#endif