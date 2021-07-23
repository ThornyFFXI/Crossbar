#ifndef __ASHITA_CrossbarItemMacro_H_INCLUDED__
#define __ASHITA_CrossbarItemMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"

class CrossbarItemMacro : public CrossbarMacro
{
private:
    IItem* pItem;
    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsBlocked;
    bool mIsTransparent;
    bool mIsTriggered;
    int mCount;
    int mRecast;

public:
    CrossbarItemMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarItemMacro();

    int GetItemCount();
    int GetItemRecast();
    bool Draw(GdiDIB* pDIB) override;
    void LoadItemResource();
    void TriggerMacro() override;
};

#endif