#include "CrossbarEmptyMacro.h"

CrossbarEmptyMacro::CrossbarEmptyMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{}

bool CrossbarEmptyMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    mForceRedraw = false;

    if (!redraw)
        return false;

    DrawIcon(pDIB, false);

    return true;
}
void CrossbarEmptyMacro::TriggerMacro()
{
    return;
}