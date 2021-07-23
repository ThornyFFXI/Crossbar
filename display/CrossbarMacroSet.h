#ifndef __ASHITA_CrossbarMacroSet_H_INCLUDED__
#define __ASHITA_CrossbarMacroSet_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#include "../macros/CrossbarMacro.h"
#include "../macros/CrossbarAbilityMacro.h"
#include "../macros/CrossbarCommandMacro.h"
#include "../macros/CrossbarEmptyMacro.h"
#include "../macros/CrossbarItemMacro.h"
#include "../macros/CrossbarSpellMacro.h"
#include "../macros/CrossbarWeaponskillMacro.h"


//Set all icons to global default.
//Then overlay job default, palette default, palette specific, provided they are not IconType::None.
//Only reconstruct palettes when changing palettes or changing job

class CrossbarMacroSet
{
private:
    IAshitaCore* pAshitaCore;
    CrossbarSettings* pSettings;
    CrossbarBindings* pBindings;
    CrossbarMacro* pMacros[8];
    int mOffsetX;
    int mOffsetY;

public:
    bool mForceRedraw;

    CrossbarMacroSet(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings, MacroMode mode);
    ~CrossbarMacroSet();
    bool Draw(GdiDIB* pDIB);
    void TriggerMacro(MacroButton button);
};

#endif