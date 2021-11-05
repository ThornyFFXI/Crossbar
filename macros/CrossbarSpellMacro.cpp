#include "CrossbarSpellMacro.h"

std::vector<int> gAddendumBlack = {
    253, //Sleep
    260, //Dispel
    259, //Sleep II
    162, //Stone IV
    172, //Water IV
    157, //Aero IV
    147, //Fire IV
    152, //Blizzard IV
    167, //Thunder IV
    163, //Stone V
    173, //Water V
    158, //Aero V
    255, //Break
    148, //Fire V
    153, //Blizzard V
    168  //Thunder V
};
std::vector<int> gAddendumWhite = {
    14,  //Poisona
    15,  //Paralyna
    16,  //Blindna
    17,  //Silena
    20,  //Cursna
    143, //Erase
    135, //Reraise
    19,  //Viruna
    18,  //Stona
    13,  //Raise II
    141, //Reraise II
    140, //Raise III
    142  //Reraise III
};

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
                if (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasSpell(pSpell->Index))
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
        if (strcmp(mMacroSettings.IconText, "DEFAULT") == 0)
        {
            DrawName(pDIB, pSpell->Name[0]);
        }
        else
        {
            DrawName(pDIB, mMacroSettings.IconText);
        }
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
        {
            if ((mainJob != 20) || (CheckAddendum(pSpell->Index)))
            {
                return true;
            }
        }

        else if (mainJobLevel == 99)
            return true;
    }

    int subRequired = pSpell->LevelRequired[subJob];
    if ((subRequired != -1)
        && (subJobLevel >= subRequired))
    {
        return ((subJob != 20) || (CheckAddendum(pSpell->Index)));
    }

    return false;
}
bool CrossbarSpellMacro::CheckAddendum(int spell)
{
    if (std::find(gAddendumBlack.begin(), gAddendumBlack.end(), spell) != gAddendumBlack.end())
    {
        int16_t* buffs = pAshitaCore->GetMemoryManager()->GetPlayer()->GetBuffs();
        for (int x = 0; x < 32; x++)
        {
            if (buffs[x] == 402)
                return true;
        }
        return false;
    }

    if (std::find(gAddendumWhite.begin(), gAddendumWhite.end(), spell) != gAddendumWhite.end())
    {
        int16_t* buffs = pAshitaCore->GetMemoryManager()->GetPlayer()->GetBuffs();
        for (int x = 0; x < 32; x++)
        {
            if (buffs[x] == 401)
                return true;
        }
        return false;
    }

    return true;
}
void CrossbarSpellMacro::TriggerMacro()
    {
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    if (strcmp(mMacroSettings.IconCommand, "DEFAULT") == 0)
    {
        int targets = pSpell->Targets;

        //Adjust valid targets for buff songs if we have pianissimo active.
        if ((pSpell->Type == 0x05) && (pSpell->Targets == 1))
        {
            for (int x = 0; x < 32; x++)
            {
                if (pAshitaCore->GetMemoryManager()->GetPlayer()->GetBuffs()[x] == 409)
                {
                    targets = 5;
                    break;
                }
            }
        }

        //Adjust valid targets for indi spells if we have entrust active.
        if (strncmp(pSpell->Name[2], "Indi-", 5) == 0)
        {
            for (int x = 0; x < 32; x++)
            {
                if (pAshitaCore->GetMemoryManager()->GetPlayer()->GetBuffs()[x] == 584)
                {
                    targets = 5;
                    break;
                }
            }
        }

        char buffer[256];
        sprintf_s(buffer, 256, "/ma \"%s\" %s", pSpell->Name[0], (pSpell->Targets == 1) ? "<me>" : "<t>");
        pAshitaCore->GetChatManager()->QueueCommand(-1, buffer);
    }
    else
    {
        pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
    }
}