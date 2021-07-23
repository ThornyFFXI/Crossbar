#ifndef __ASHITA_CrossbarItemMacro_H_INCLUDED__
#define __ASHITA_CrossbarItemMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "CrossbarMacro.h"
#include "../display/GdiItem.h"

class CrossbarItemMacro : public CrossbarMacro
{
private:
    IItem* pItem;
    GdiItem* pGdiItem;
    std::chrono::steady_clock::time_point mActivationTime;
    bool mIsBlocked;
    bool mIsTransparent;
    bool mIsTriggered;
    int mCount;
    int mRecast;

public:
    const uint32_t mVanadielOffset = 0x3C307D70;
    static DWORD pRealTime;

    CrossbarItemMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarItemMacro();

    int GetItemCount();
    int GetItemRecast();
    uint32_t GetRealTime();
    bool Draw(GdiDIB* pDIB) override;
    void LoadItemResource();
    void TriggerMacro() override;
};

#endif