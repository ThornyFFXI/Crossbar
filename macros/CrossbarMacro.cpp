#include "CrossbarMacro.h"


CrossbarMacro::CrossbarMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : pAshitaCore(pAshitaCore)
    , pSettings(pSettings)
    , mMacroSettings(macroSettings)
{
    if (mMacroSettings.DrawCost == DrawSetting::Default)
    {
        mMacroSettings.DrawCost = (DrawSetting)((pSettings->mConfig.EnableCost) && (pSettings->pMacro->pCost != NULL));
    }
    if (mMacroSettings.DrawCross == DrawSetting::Default)
    {
        mMacroSettings.DrawCross = (DrawSetting)(pSettings->mConfig.EnableCross);
    }
    if (mMacroSettings.DrawFade == DrawSetting::Default)
    {
        mMacroSettings.DrawFade = (DrawSetting)(pSettings->mConfig.EnableFade);
    }
    if (mMacroSettings.DrawName == DrawSetting::Default)
    {
        mMacroSettings.DrawName = (DrawSetting)((pSettings->mConfig.EnableName) && (pSettings->pMacro->pName != NULL));
    }
    if (mMacroSettings.DrawRecast == DrawSetting::Default)
    {
        mMacroSettings.DrawRecast = (DrawSetting)((pSettings->mConfig.EnableRecast) && (pSettings->pMacro->pRecast != NULL));
    }
    if (mMacroSettings.DrawSkillchain == DrawSetting::Default)
    {
        mMacroSettings.DrawSkillchain = (DrawSetting)(pSettings->mConfig.EnableSkillchain);
    }
    if (mMacroSettings.DrawTrigger == DrawSetting::Default)
    {
        mMacroSettings.DrawTrigger = (DrawSetting)(pSettings->mConfig.EnableTrigger);
    }

    pCrossBrush = new Gdiplus::SolidBrush(Gdiplus::Color(128, 255, 0, 0));
    pCrossPen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 1);
    pSkillchainBrush = new Gdiplus::SolidBrush(Gdiplus::Color(128, 0, 255, 0));
    pImage = NULL;

    int xOffset = 0;
    if (offset)
    {
        xOffset = pSettings->pSubPanel->PanelWidth + pSettings->pSubPanel->PanelSpacing;
    }

    mMacroRect = Gdiplus::Rect(xOffset + pSettings->pSubPanel->MacroOffsetX[index], pSettings->pSubPanel->MacroOffsetY[index], pSettings->pSubPanel->MacroWidth, pSettings->pSubPanel->MacroHeight);
    mFrameRect = Gdiplus::Rect(mMacroRect.X + pSettings->pSubPanel->IconOffsetX, mMacroRect.Y + pSettings->pSubPanel->IconOffsetY, pSettings->pSubPanel->IconWidth, pSettings->pSubPanel->IconHeight);
    mIconRect = Gdiplus::Rect(mMacroRect.X + pSettings->pSubPanel->IconOffsetX, mMacroRect.Y + pSettings->pSubPanel->IconOffsetY, pSettings->pSubPanel->IconWidth, pSettings->pSubPanel->IconHeight);
    
    if (mMacroSettings.DrawFade == DrawSetting::Draw)
    {
        float opacity = (1.0f - (static_cast<float>(pSettings->mConfig.FadeAlpha) / 255.0f));
        pMatrix = new Gdiplus::ColorMatrix{
               1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f, opacity, 0.0f,
               0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    else
    {
        pMatrix = NULL;
    }

    if (mMacroSettings.DrawCross == DrawSetting::Draw)
    {
        int thickness = 4;
        int middleX = mIconRect.X + (mIconRect.Width / 2);
        int farX = mIconRect.X + mIconRect.Width;
        int middleY = mIconRect.Y + (mIconRect.Height / 2);
        int farY = mIconRect.Y + mIconRect.Height;
        pCrossPath = new Gdiplus::GraphicsPath();
        pCrossPath->AddLine(mIconRect.X, mIconRect.Y, mIconRect.X + thickness, mIconRect.Y);
        pCrossPath->AddLine(mIconRect.X + thickness, mIconRect.Y, middleX, middleY - (thickness));
        pCrossPath->AddLine(middleX, middleY - (thickness), farX - thickness, mIconRect.Y);
        pCrossPath->AddLine(farX - thickness, mIconRect.Y, farX, mIconRect.Y);
        pCrossPath->AddLine(farX, mIconRect.Y, farX, mIconRect.Y + thickness);
        pCrossPath->AddLine(farX, mIconRect.Y + thickness, middleX + (thickness), middleY);
        pCrossPath->AddLine(middleX + (thickness), middleY, farX, farY - thickness);
        pCrossPath->AddLine(farX, farY - thickness, farX, farY);
        pCrossPath->AddLine(farX, farY, farX - thickness, farY);
        pCrossPath->AddLine(farX - thickness, farY, middleX, middleY + (thickness));
        pCrossPath->AddLine(middleX, middleY + (thickness), mIconRect.X + thickness, farY);
        pCrossPath->AddLine(mIconRect.X + thickness, farY, mIconRect.X, farY);
        pCrossPath->AddLine(mIconRect.X, farY, mIconRect.X, farY - thickness);
        pCrossPath->AddLine(mIconRect.X, farY - thickness, middleX - (thickness), middleY);
        pCrossPath->AddLine(middleX - (thickness), middleY, mIconRect.X, mIconRect.Y + thickness);
        pCrossPath->AddLine(mIconRect.X, mIconRect.Y + thickness, mIconRect.X, mIconRect.Y);
        pCrossPath->CloseFigure();
    }
    else
    {
        pCrossPath = NULL;
    }

    if (pSettings->pMacro->pCost)
    {
        pCostPath = NULL;
        mCostRect = Gdiplus::Rect(mMacroRect.X + pSettings->pMacro->pCost->OffsetX, mMacroRect.Y + pSettings->pMacro->pCost->OffsetY, pSettings->pMacro->pCost->Width, pSettings->pMacro->pCost->Height);
    }
    if (pSettings->pMacro->pRecast)
    {
        pRecastPath = NULL;
        mRecastRect = Gdiplus::Rect(mMacroRect.X + pSettings->pMacro->pRecast->OffsetX, mMacroRect.Y + pSettings->pMacro->pRecast->OffsetY, pSettings->pMacro->pRecast->Width, pSettings->pMacro->pRecast->Height);
    }
    if (pSettings->pMacro->pName)
    {
        pNamePath = NULL;
        mNameRect = Gdiplus::Rect(mMacroRect.X + pSettings->pMacro->pName->OffsetX, mMacroRect.Y + pSettings->pMacro->pName->OffsetY, pSettings->pMacro->pName->Width, pSettings->pMacro->pName->Height);
    }
    mForceRedraw = true;
    mImageAllocated = false;
}
CrossbarMacro::~CrossbarMacro()
{
    delete pCrossBrush;
    delete pCrossPen;
    if (pMatrix)
    {
        delete pMatrix;
    }
    if (pCrossPath)
    {
        delete pCrossPath;
    }
    if (pCostPath)
    {
        delete pCostPath;
    }
    if (pRecastPath)
    {
        delete pRecastPath;
    }
    if (pNamePath)
    {
        delete pNamePath;
    }
    if (mImageAllocated)
    {
        delete pImage;
    }
}

void CrossbarMacro::DrawCross(GdiDIB* pDIB)
{
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }

    pDIB->GetGraphics()->DrawPath(pCrossPen, pCrossPath);
    pDIB->GetGraphics()->FillPath(pCrossBrush, pCrossPath);
}
void CrossbarMacro::DrawCost(GdiDIB* pDIB, int cost)
{
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }
    if (pSettings->pMacro->pCost == NULL)
    {
        return;
    }

    wchar_t textBuffer[1024];
    swprintf_s(textBuffer, 1024, L"%d", cost);
    if (wcscmp(textBuffer, mCostText) != 0)
    {
        wcsncpy_s(mCostText, textBuffer, 1024);
        if (pCostPath)
        {
            delete pCostPath;
        }
        pCostPath = new Gdiplus::GraphicsPath();
        pSettings->pMacro->pCost->AddToPath(pCostPath, mCostText, mCostRect);
    }
    pDIB->GetGraphics()->DrawPath(pSettings->pMacro->pCost->pFont->pPen, pCostPath);
    pDIB->GetGraphics()->FillPath(pSettings->pMacro->pCost->pFont->pBrush, pCostPath);
}
void CrossbarMacro::DrawIcon(GdiDIB* pDIB, bool fade)
{
    pDIB->ClearRegion(mMacroRect.X, mMacroRect.Y, mMacroRect.Width, mMacroRect.Height);
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }

    if (pSettings->pSubPanel->pFrame)
    {
        pDIB->GetGraphics()->DrawImage(pSettings->pSubPanel->pFrame, mFrameRect, 0, 0, pSettings->pSubPanel->pFrame->GetWidth(), pSettings->pSubPanel->pFrame->GetHeight(), Gdiplus::Unit::UnitPixel);
    }
    if (pImage)
    {
        if (fade)
        {
            Gdiplus::ImageAttributes attributes;
            attributes.SetColorMatrix(pMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);
            pDIB->GetGraphics()->DrawImage(pImage, mIconRect, 0, 0, pImage->GetWidth(), pImage->GetHeight(), Gdiplus::Unit::UnitPixel, &attributes);
        }
        else
        {
            pDIB->GetGraphics()->DrawImage(pImage, mIconRect);
        }
    }
    else
    {
        pDIB->GetGraphics()->FillRectangle(pSettings->pMacro->pBackgroundBrush, mIconRect);
    }
}
void CrossbarMacro::DrawName(GdiDIB* pDIB, const char* text)
{
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }
    if (pSettings->pMacro->pName == NULL)
    {
        return;
    }

    wchar_t textBuffer[1024];
    swprintf_s(textBuffer, 1024, L"%S", text);
    if (wcscmp(textBuffer, mNameText) != 0)
    {
        wcsncpy_s(mNameText, textBuffer, 1024);
        if (pNamePath)
        {
            delete pNamePath;
        }
        pNamePath = new Gdiplus::GraphicsPath();
        pSettings->pMacro->pName->AddToPath(pNamePath, mNameText, mNameRect);
    }
    pDIB->GetGraphics()->DrawPath(pSettings->pMacro->pName->pFont->pPen, pNamePath);
    pDIB->GetGraphics()->FillPath(pSettings->pMacro->pName->pFont->pBrush, pNamePath);
}
void CrossbarMacro::DrawRecast(GdiDIB* pDIB, int recast)
{
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }
    if (pSettings->pMacro->pRecast == NULL)
    {
        return;
    }

    wchar_t textBuffer[1024];
    swprintf_s(textBuffer, 1024, L"%d", recast);
    if (wcscmp(textBuffer, mRecastText) != 0)
    {
        wcsncpy_s(mRecastText, textBuffer, 1024);
        if (pRecastPath)
        {
            delete pRecastPath;
        }
        pRecastPath = new Gdiplus::GraphicsPath();
        pSettings->pMacro->pRecast->AddToPath(pRecastPath, mRecastText, mRecastRect);
    }
    pDIB->GetGraphics()->DrawPath(pSettings->pMacro->pRecast->pFont->pPen, pRecastPath);
    pDIB->GetGraphics()->FillPath(pSettings->pMacro->pRecast->pFont->pBrush, pRecastPath);
}
void CrossbarMacro::DrawSkillchainCrawl(GdiDIB* pDIB, int skillchainStep)
{
    if (pSettings->pMacro->pCrawl[skillchainStep])
    {
        pDIB->GetGraphics()->DrawImage(pSettings->pMacro->pCrawl[skillchainStep], mIconRect);
    }
}
void CrossbarMacro::DrawSkillchainProgress(GdiDIB* pDIB, int width)
{
    Gdiplus::Rect newRect = mIconRect;
    newRect.Height = 8;
    newRect.Width = width;
    pDIB->GetGraphics()->FillRectangle(pSkillchainBrush, newRect);
}
void CrossbarMacro::DrawTrigger(GdiDIB* pDIB)
{
    if ((mMacroSettings.Type == IconType::None) && (pSettings->mConfig.HideEmptyIcons))
    {
        return;
    }

    pDIB->GetGraphics()->FillRectangle(pSettings->pMacro->pTriggerBrush, mIconRect);
}
bool CrossbarMacro::LoadImageFromFullPath(std::string path)
{
    if (!std::filesystem::exists(path))
        return false;

    wchar_t textBuffer[1024];
    swprintf_s(textBuffer, 1024, L"%S", path.c_str());
    try
    {
        pImage = new Gdiplus::Image(textBuffer);
        mImageAllocated = true;
        return true;
    }
    catch (...)
    {
        pImage = NULL;
    }
    return false;
}