#ifndef __ASHITA_CrossbarMacro_H_INCLUDED__
#define __ASHITA_CrossbarMacro_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#include "../config/CrossbarBindings.h"
#include "../config/CrossbarSettings.h"
#include "../display/GdiDIB.h"

class CrossbarMacro
{
private:
    Gdiplus::Brush* pCrossBrush;
    Gdiplus::Pen* pCrossPen;
    Gdiplus::GraphicsPath* pCrossPath;

    Gdiplus::Brush* pSkillchainBrush;

    wchar_t mCostText[1024];
    Gdiplus::GraphicsPath* pCostPath;
    Gdiplus::Rect mCostRect;

    wchar_t mNameText[1024];
    Gdiplus::GraphicsPath* pNamePath;
    Gdiplus::Rect mNameRect;

    wchar_t mRecastText[1024];
    Gdiplus::GraphicsPath* pRecastPath;
    Gdiplus::Rect mRecastRect;

    Gdiplus::Image* pBackground;
    Gdiplus::ColorMatrix* pMatrix;
    Gdiplus::Rect mMacroRect;
    Gdiplus::Rect mFrameRect;
    Gdiplus::Rect mIconRect;

public:
    IAshitaCore* pAshitaCore;
    CrossbarSettings* pSettings;
    SingleMacroInfo_t mMacroSettings;
    Gdiplus::Image* pImage;
    bool mForceRedraw;
    bool mImageAllocated;

    CrossbarMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index);
    ~CrossbarMacro();
    virtual bool Draw(GdiDIB* pDIB) = 0;
    virtual void TriggerMacro() = 0;

    void DrawCross(GdiDIB* pDIB);
    void DrawCost(GdiDIB* pDIB, int cost);
    void DrawIcon(GdiDIB* pDIB, bool fade);
    void DrawName(GdiDIB* pDIB, const char* text);
    void DrawRecast(GdiDIB* pDIB, int recast);
    void DrawSkillchainCrawl(GdiDIB* pDIB, int skillchainStep);
    void DrawSkillchainProgress(GdiDIB* pDIB, int width);
    void DrawTrigger(GdiDIB* pDIB);
    bool LoadImageFromFullPath(std::string path);
    void Update(SingleMacroInfo_t mMacroSettings);
};

#endif