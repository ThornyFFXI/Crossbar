#include "CrossbarCommandMacro.h"

CrossbarCommandMacro::CrossbarCommandMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{
    std::list<std::string> paths;
    char buffer[256];
    sprintf_s(buffer, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), macroSettings.IconFile);
    paths.push_back(std::string(buffer));
    for (std::list<std::string>::iterator iter = paths.begin(); iter != paths.end(); iter++)
    {
        if (LoadImageFromFullPath(*iter))
        {
            break;
        }
    }
    if (pImage == NULL)
    {
        pImage = pSettings->pMacro->pDefaultCommandIcon;
    }
}

bool CrossbarCommandMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    mForceRedraw = false;

    bool isTriggered = false;
    if (mMacroSettings.DrawTrigger == DrawSetting::Draw)
    {
        if (std::chrono::steady_clock::now() < mActivationTime)
        {
            isTriggered = true;
        }
    }
    if (isTriggered != mIsTriggered)
    {
        mIsTriggered = isTriggered;
        redraw = true;
    }

    if (!redraw)
        return false;

    DrawIcon(pDIB, false);

    if (mIsTriggered)
    {
        DrawTrigger(pDIB);
    }

    if (mMacroSettings.DrawName == DrawSetting::Draw)
    {
        if (strcmp(mMacroSettings.IconText, "DEFAULT") == 0)
        {
            DrawName(pDIB, mMacroSettings.IconCommand);
        }
        else
        {
            DrawName(pDIB, mMacroSettings.IconText);
        }
    }

    return true;
}
void CrossbarCommandMacro::TriggerMacro()
{
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
}