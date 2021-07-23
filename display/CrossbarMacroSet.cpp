#include "CrossbarMacroSet.h"

CrossbarMacroSet::CrossbarMacroSet(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings, MacroMode mode)
    : pAshitaCore(pAshitaCore)
    , pSettings(pSettings)
    , pBindings(pBindings)
{
    if (mode == MacroMode::RightTrigger)
    {
        mOffsetX = pSettings->pSubPanel->PanelWidth + pSettings->pSubPanel->PanelSpacing;
        mOffsetY = 0;
    }
    else
    {
        mOffsetX = 0;
        mOffsetY = 0;
    }

    SingleMacroInfo_t* macros[8] = { 0 };
    SingleMacroInfo_t* layers[3];
    layers[0] = pBindings->pGlobalDefaults->Triggers[(int)mode].Buttons;
    layers[1] = pBindings->pJobSettings->pJobDefaults->Triggers[(int)mode].Buttons;
    layers[2] = (*pBindings->pJobSettings->mPaletteIterator)->pBindings->Triggers[(int)mode].Buttons;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (layers[y][x].Type != IconType::None)
            {
                macros[x] = &(layers[y][x]);
            }
        }
        if (pBindings->pDisplayPalette != NULL)
        {
            macros[x] = &(pBindings->pDisplayPalette->Triggers[(int)mode].Buttons[x]);
        }

        if (macros[x] == NULL) 
        {
            pMacros[x] = new CrossbarEmptyMacro(pAshitaCore, pSettings, SingleMacroInfo_t(), (mode == MacroMode::RightTrigger), x);
        }
        else
        {
            SingleMacroInfo_t* info = macros[x];
            if (info->Type == IconType::Ability)
                pMacros[x] = new CrossbarAbilityMacro(pAshitaCore, pSettings, *info, (mode == MacroMode::RightTrigger),  x);
            else if (info->Type == IconType::Command)
                pMacros[x] = new CrossbarCommandMacro(pAshitaCore, pSettings, *info, (mode == MacroMode::RightTrigger), x);
            else if (info->Type == IconType::Item)
                pMacros[x] = new CrossbarItemMacro(pAshitaCore, pSettings, *info, (mode == MacroMode::RightTrigger), x);
            else if (info->Type == IconType::Spell)
                pMacros[x] = new CrossbarSpellMacro(pAshitaCore, pSettings, *info, (mode == MacroMode::RightTrigger), x);
            else if (info->Type == IconType::Weaponskill)
                pMacros[x] = new CrossbarWeaponskillMacro(pAshitaCore, pSettings, *info, (mode == MacroMode::RightTrigger), x);
            else
                pMacros[x] = new CrossbarEmptyMacro(pAshitaCore, pSettings, SingleMacroInfo_t(), (mode == MacroMode::RightTrigger), x);
        }
    }

    mForceRedraw = true;
}
CrossbarMacroSet::~CrossbarMacroSet()
{
    for (int x = 0; x < 8; x++)
    {
        if (pMacros[x])
        {
            delete pMacros[x];
        }
    }
}

bool CrossbarMacroSet::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    mForceRedraw = false;

    if (redraw)
    {
        if (pSettings->pSubPanel->pButtons != NULL)
        {
            pDIB->GetGraphics()->DrawImage(pSettings->pSubPanel->pButtons, Gdiplus::Rect(mOffsetX + pSettings->pSubPanel->ButtonsOffsetX, mOffsetY + pSettings->pSubPanel->ButtonsOffsetY, pSettings->pSubPanel->ButtonsWidth, pSettings->pSubPanel->ButtonsHeight));
        }
        if (pSettings->pSubPanel->pDpad != NULL)
        {
            pDIB->GetGraphics()->DrawImage(pSettings->pSubPanel->pDpad, Gdiplus::Rect(mOffsetX + pSettings->pSubPanel->DpadOffsetX, mOffsetY + pSettings->pSubPanel->DpadOffsetY, pSettings->pSubPanel->DpadWidth, pSettings->pSubPanel->DpadHeight));
        }
    }

    for (int x = 0; x < 8; x++)
    {
        if (pMacros[x]->Draw(pDIB))
        {
            redraw = true;
        }
    }

    return redraw;
}

void CrossbarMacroSet::TriggerMacro(MacroButton button)
{
    pMacros[(int)button]->TriggerMacro();
}