#ifndef __ASHITA_FontMenuUI_H_INCLUDED__
#define __ASHITA_FontMenuUI_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "FontMenuDefines.h"
#include "../display/GdiDIB.h"
#include "../config/CrossbarSettings.h"
#include "C:\Ashita 4\plugins\sdk\Ashita.h"

class FontMenuUI
{
private:
    IAshitaCore* pAshitaCore;
    IPrimitiveObject* pPrimitiveObject;
    GdiDIB* pDIB;
    CrossbarSettings* pSettings;

public:
    FontMenuUI(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, const char* name);
    ~FontMenuUI();

    void Draw(FontMenuState* pState);

private:
    void DrawInputPrompt(FontMenuState* pState);
    void DrawOptions(FontMenuState* pState);
    void DrawOption(int index, const char* label, const char* value);
    void DrawPrompt(FontMenuState* pState);
    int DrawLabels(FontMenuState* pState);
};

#endif