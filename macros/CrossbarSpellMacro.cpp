#include "CrossbarSpellMacro.h"

CrossbarSpellMacro::CrossbarSpellMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{
    pSpell = reinterpret_cast<ISpell*>(macroSettings.pResource);
    std::list<std::string> paths;
    char buffer[256];
    sprintf_s(buffer, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), macroSettings.IconFile);
    paths.push_back(std::string(buffer));
    if (pSpell)
    {
        sprintf_s(buffer, 256, "%sresources//crossbar//spells/%u.png", pAshitaCore->GetInstallPath(), pSpell->Index);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//spells/%u.bmp", pAshitaCore->GetInstallPath(), pSpell->Index);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//spells/%u.jpg", pAshitaCore->GetInstallPath(), pSpell->Index);
        paths.push_back(std::string(buffer));
    }
    for (std::list<std::string>::iterator iter = paths.begin(); iter != paths.end(); iter++)
    {
        if (LoadImageFromFullPath(*iter))
        {
            break;
        }
    }
    if (pImage == NULL)
    {
        pImage = pSettings->pMacro->pDefaultSpellIcon;
    }
}

bool CrossbarSpellMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;

    int recast = -1;
    if (pSpell)
    {
        int rawRecast = pAshitaCore->GetMemoryManager()->GetRecast()->GetSpellTimer(pSpell->Index);
        recast = (rawRecast / 60) + (((rawRecast % 60) != 0) ? 1 : 0);
    }
    if (mMacroSettings.DrawRecast == DrawSetting::Draw)
    {
        if (recast != mRecast)
        {
            redraw = true;
        }
    }
    mRecast = recast;

    bool isTransparent = false;
    if (pSpell)
    {
        if (mMacroSettings.DrawFade == DrawSetting::Draw)
        {
            if (pAshitaCore->GetMemoryManager()->GetParty()->GetMemberMP(0) < pSpell->ManaCost)
            {
                isTransparent = true;
            }
            if (recast != 0)
            {
                isTransparent = true;
            }
        }
    }
    if (isTransparent != mIsTransparent)
    {
        mIsTransparent = isTransparent;
        redraw = true;
    }

    bool isBlocked = false;
    if (pSpell)
    {
        if (mMacroSettings.DrawCross == DrawSetting::Draw)
        {
            try
            {
                if (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasSpell(pSpell->Id))
                {
                    isBlocked = true;
                }
            }
            catch (...)
            {
                isBlocked = true;
            }         

            if (!CheckSpellAvailable())
            {
                isBlocked = true;
            }

            for (std::list<int16_t>::iterator iter = mRequiredBuffs.begin(); iter != mRequiredBuffs.end(); iter++)
            {
                bool buffFound = false;
                int16_t* buffs = pAshitaCore->GetMemoryManager()->GetPlayer()->GetBuffs();
                for (int x = 0; x < 32; x++)
                {
                    if (buffs[x] = *iter)
                    {
                        buffFound = true;
                        break;
                    }
                }
                if (!buffFound)
                {
                    isBlocked = true;
                }
            }
        }
    }
    if (isBlocked != mIsBlocked)
    {
        mIsBlocked = isBlocked;
        redraw = true;
    }

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

    DrawIcon(pDIB, isTransparent);

    if (mIsBlocked)
    {
        DrawCross(pDIB);
    }

    if (mIsTriggered)
    {
        DrawTrigger(pDIB);
    }

    if ((mMacroSettings.DrawCost == DrawSetting::Draw) && (pSpell->ManaCost != 0))
    {
        DrawCost(pDIB, static_cast<int>(pSpell->ManaCost));
    }

    if ((mMacroSettings.DrawRecast == DrawSetting::Draw) && (recast != 0))
    {
        DrawRecast(pDIB, recast);
    }

    if (mMacroSettings.DrawName == DrawSetting::Draw)
    {
        DrawName(pDIB, mMacroSettings.IconText);
    }

    return true;
}
bool CrossbarSpellMacro::CheckSpellAvailable()
{
    int mainJob = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJob();
    int mainJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJobLevel();
    int subJob = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJob();
    int subJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJobLevel();

    int mainRequired = pSpell->LevelRequired[mainJob];
    if (mainRequired != -1)
    {
        if (mainJobLevel >= mainRequired)
            return true;

        if (mainJobLevel == 99)
            return true;
    }

    int subRequired = pSpell->LevelRequired[subJob];
    return ((subRequired != -1) && (subJobLevel >= subRequired));
}
void CrossbarSpellMacro::TriggerMacro()
{
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
}