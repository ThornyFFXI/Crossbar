#include "CrossbarCanvas.h"

CrossbarCanvas::CrossbarCanvas(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings)
    : pAshitaCore(pAshitaCore)
    , pBindings(pBindings)
    , pSettings(pSettings)
{
    pMainDisplay = new GdiDIB(pAshitaCore->GetDirect3DDevice(), (pSettings->pSubPanel->PanelWidth * 2) + pSettings->pSubPanel->PanelSpacing, pSettings->pSubPanel->PanelHeight);
    pMainDisplay->GetGraphics()->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    pMainDisplay->GetGraphics()->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    pMainDisplay->GetGraphics()->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
    pMainPrimitive = pAshitaCore->GetPrimitiveManager()->Create("CrossbarMain");
    pSubDisplay = new GdiDIB(pAshitaCore->GetDirect3DDevice(), pSettings->pSubPanel->PanelWidth, pSettings->pSubPanel->PanelHeight);
    pSubDisplay->GetGraphics()->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    pSubDisplay->GetGraphics()->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    pSubDisplay->GetGraphics()->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
    pSubPrimitive = pAshitaCore->GetPrimitiveManager()->Create("CrossbarSub");
    mLastSingleMode = MacroMode::NoTrigger;
    
    RECT rect;
    GetWindowRect(pAshitaCore->GetProperties()->GetFinalFantasyHwnd(), &rect);

    if (pSettings->mConfig.MainPanelX == -1)
    {
        pMainPrimitive->SetPositionX((rect.right - rect.left - ((pSettings->pSubPanel->PanelWidth * 2) + pSettings->pSubPanel->PanelSpacing)) / 2);
    }
    else
    {
        pMainPrimitive->SetPositionX(pSettings->mConfig.MainPanelX);
    }
    if (pSettings->mConfig.MainPanelY < 0)
    {
        pMainPrimitive->SetPositionY((rect.bottom - rect.top) + pSettings->mConfig.MainPanelY - pSettings->pSubPanel->PanelHeight);
    }
    else
    {
        pMainPrimitive->SetPositionY(pSettings->mConfig.MainPanelY);
    }

    if (pSettings->mConfig.SubPanelX == -1)
    {
        pSubPrimitive->SetPositionX((rect.right - rect.left - pSettings->pSubPanel->PanelWidth) / 2);
    }
    else
    {
        pSubPrimitive->SetPositionX(pSettings->mConfig.SubPanelX);
    }
    if (pSettings->mConfig.SubPanelY < 0)
    {
        pSubPrimitive->SetPositionY((rect.bottom - rect.top) + pSettings->mConfig.SubPanelY - pSettings->pSubPanel->PanelHeight);
    }
    else
    {
        pSubPrimitive->SetPositionY(pSettings->mConfig.SubPanelY);
    }

    for (int x = 0; x < 6; x++)
    {
        pMacros[x] = new CrossbarMacroSet(pAshitaCore, pSettings, pBindings, (MacroMode)x);
    }
}
CrossbarCanvas::~CrossbarCanvas()
{
    pAshitaCore->GetPrimitiveManager()->Delete("CrossbarSub");
    delete pSubDisplay;
    pAshitaCore->GetPrimitiveManager()->Delete("CrossbarMain");
    delete pMainDisplay;
    for (int x = 0; x < 6; x++)
    {
        delete pMacros[x];
    }
}

void CrossbarCanvas::Draw(MacroMode mode)
{
    if (pBindings->mRedraw)
    {
        UpdatePalette();
        pBindings->mRedraw = false;
    }

    if ((mode == MacroMode::LeftTrigger) || (mode == MacroMode::RightTrigger) || (mode == MacroMode::NoTrigger))
    {
        pSubPrimitive->SetVisible(false);
        bool reapply = pMacros[(int)MacroMode::LeftTrigger]->Draw(pMainDisplay);
        if (pMacros[(int)MacroMode::RightTrigger]->Draw(pMainDisplay) || reapply)
        {
            pMainDisplay->ApplyToPrimitiveObject(pMainPrimitive);
        }
        pMainPrimitive->SetVisible(true);
    }
    else
    {
        if (mLastSingleMode != mode)
        {
            pMacros[(int)mode]->ForceMacroRedraw();
            mLastSingleMode = mode;
        }

        pMainPrimitive->SetVisible(false);
        if (pMacros[(int)mode]->Draw(pSubDisplay))
        {
            pSubDisplay->ApplyToPrimitiveObject(pSubPrimitive);
        }
        pSubPrimitive->SetVisible(true);
    }
}
void CrossbarCanvas::HandleButton(MacroButton button, MacroMode mode)
{
    pMacros[(int)mode]->TriggerMacro(button);
}
void CrossbarCanvas::Hide()
{
    pMainPrimitive->SetVisible(false);
    pSubPrimitive->SetVisible(false);
}
void CrossbarCanvas::UpdateBindings(CrossbarBindings* pNewBindings)
{
    delete pBindings;
    pBindings = pNewBindings;
    for (int x = 0; x < 6; x++)
    {
        delete pMacros[x];
        pMacros[x] = new CrossbarMacroSet(pAshitaCore, pSettings, pBindings, (MacroMode)x);
    }
}
void CrossbarCanvas::UpdatePalette()
{
    for (int x = 0; x < 6; x++)
    {
        delete pMacros[x];
        pMacros[x] = new CrossbarMacroSet(pAshitaCore, pSettings, pBindings, (MacroMode)x);
    }
}