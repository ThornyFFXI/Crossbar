#include "CrossbarAbilityMacro.h"

CrossbarAbilityMacro::CrossbarAbilityMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{
    pAbility = reinterpret_cast<IAbility*>(macroSettings.pResource);
    std::list<std::string> paths;
    char buffer[256];
    sprintf_s(buffer, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), macroSettings.IconFile);
    paths.push_back(std::string(buffer));
    if (pAbility)
    {
        sprintf_s(buffer, 256, "%sresources//crossbar//abilities/%u.png", pAshitaCore->GetInstallPath(), pAbility->Id - 512);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//abilities/%u.bmp", pAshitaCore->GetInstallPath(), pAbility->Id - 512);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//abilities/%u.jpg", pAshitaCore->GetInstallPath(), pAbility->Id - 512);
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
        pImage = pSettings->pMacro->pDefaultAbilityIcon;
    }
}

bool CrossbarAbilityMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    mForceRedraw = false;

    int recast = -1;
    if (pAbility)
    {
        if (pAbility->RecastTimerId == 0)
        {
            int rawRecast = pAshitaCore->GetMemoryManager()->GetRecast()->GetAbilityTimer(0);
            recast = (rawRecast / 60) + (((rawRecast % 60) != 0) ? 1 : 0);
        }
        else
        {
            for (int x = 0; x < 32; x++)
            {
                if (pAshitaCore->GetMemoryManager()->GetRecast()->GetAbilityTimerId(x) == pAbility->RecastTimerId)
                {
                    int rawRecast = pAshitaCore->GetMemoryManager()->GetRecast()->GetAbilityTimer(x);
                    recast = (rawRecast / 60) + (((rawRecast % 60) != 0) ? 1 : 0);
                }
            }
        }
        if (recast < 1)
            recast = 0;
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
    if (pAbility)
    {
        if (mMacroSettings.DrawFade == DrawSetting::Draw)
        {
            if (pAshitaCore->GetMemoryManager()->GetParty()->GetMemberTP(0) < pAbility->TPCost)
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
    if (pAbility)
    {
        if (mMacroSettings.DrawCross == DrawSetting::Draw)
        {
            try
            {
                if (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasAbility(pAbility->Id))
                {
                    isBlocked = true;
                }
            }
            catch (...)
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

    if ((mMacroSettings.DrawCost == DrawSetting::Draw) && (pAbility->TPCost > 0))
    {
        DrawCost(pDIB, static_cast<int>(pAbility->TPCost));
    }

    if ((mMacroSettings.DrawRecast == DrawSetting::Draw) && (recast != 0))
    {
        DrawRecast(pDIB, recast);
    }

    if (mMacroSettings.DrawName == DrawSetting::Draw)
    {
        if (strcmp(mMacroSettings.IconText, "DEFAULT") == 0)
        {
            DrawName(pDIB, pAbility->Name[0]);
        }
        else
        {
            DrawName(pDIB, mMacroSettings.IconText);
        }
    }

    return true;
}
void CrossbarAbilityMacro::TriggerMacro()
{
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    if (strcmp(mMacroSettings.IconCommand, "DEFAULT") == 0)
    {
        char buffer[256];
        sprintf_s(buffer, 256, "/ja \"%s\" %s", pAbility->Name[0], (pAbility->Targets == 1) ? "<me>" : "<t>");
        pAshitaCore->GetChatManager()->QueueCommand(-1, buffer);
    }
    else
    {
        pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
    }
}