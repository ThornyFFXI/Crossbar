#include "CrossbarWeaponskillMacro.h"
#include "CrossbarWeaponskillMacro.h"

CrossbarWeaponskillMacro::CrossbarWeaponskillMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{
    pAbility = reinterpret_cast<IAbility*>(macroSettings.pResource);
    std::list<std::string> paths;
    char buffer[256];
    sprintf_s(buffer, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), macroSettings.IconFile);
    paths.push_back(std::string(buffer));
    if (pAbility)
    {
        sprintf_s(buffer, 256, "%sresources//crossbar//weaponskills/%u.png", pAshitaCore->GetInstallPath(), pAbility->Id);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//weaponskills/%u.bmp", pAshitaCore->GetInstallPath(), pAbility->Id);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//weaponskills/%u.jpg", pAshitaCore->GetInstallPath(), pAbility->Id);
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
        pImage = pSettings->pMacro->pDefaultWeaponskillIcon;
    }
}

bool CrossbarWeaponskillMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    int skillchainStep = -1; //Always -1 when no sc.
    int skillchainProgress = 0;
    Gdiplus::Image* pDefaultImage = pImage;
    SkillchainIndex index = pResonation->GetResult(pAbility);
    WindowState state = pResonation->GetWindow();

    if (mMacroSettings.DrawSkillchain == DrawSetting::Draw)
    {
        if ((index != SkillchainIndex::None) && (state != WindowState::Expired))
        {
            if (state == WindowState::NotOpen)
            {
                skillchainStep = 0; //Always 0 when waiting for window to open.
                int ms = pResonation->GetTimeUntilOpen();
                if ((ms < 1) || (ms > 3500))
                {
                    skillchainProgress = 0;
                }
                else
                {
                    int ratio = ((3500 - ms) * 100) / 3500;
                    skillchainProgress = min(pSettings->pSubPanel->IconWidth, (pSettings->pSubPanel->IconWidth * ratio) / 100);
                }
            }
            else
            {
                skillchainStep = mSkillchainStep;
                if (skillchainStep <= 0)
                {
                    skillchainStep = 1;
                    mFrameAdvance = std::chrono::steady_clock::now() + std::chrono::milliseconds(80);
                }
                else if (std::chrono::steady_clock::now() > mFrameAdvance)
                {
                    skillchainStep = (skillchainStep == 7) ? 1 : (skillchainStep + 1);
                    mFrameAdvance = std::chrono::steady_clock::now() + std::chrono::milliseconds(80);
                }
            }
            pImage = pSettings->pMacro->pSkillchain[(int)index];
        }
    }

    bool isTransparent = false;
    if (pAbility)
    {
        if (mMacroSettings.DrawFade == DrawSetting::Draw)
        {
            if (pAshitaCore->GetMemoryManager()->GetParty()->GetMemberTP(0) < 1000)
            {
                isTransparent = true;
            }
            if ((skillchainStep != -1) && (state == WindowState::NotOpen))
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

    if (skillchainStep != mSkillchainStep)
    {
        mSkillchainStep = skillchainStep;
        redraw = true;
    }
    if (skillchainProgress != mSkillchainProgress)
    {
        mSkillchainProgress = skillchainProgress;
    }

    if (!redraw)
    {
        pImage = pDefaultImage;
        return false;
    }

    DrawIcon(pDIB, isTransparent);
    pImage = pDefaultImage;

    if (mIsBlocked)
    {
        DrawCross(pDIB);
    }

    if (mIsTriggered)
    {
        DrawTrigger(pDIB);
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

    if (mSkillchainStep > 0)
    {
        DrawSkillchainCrawl(pDIB, mSkillchainStep - 1);
    }
    else if (mSkillchainProgress != 0)
    {
        DrawSkillchainProgress(pDIB, mSkillchainProgress);
    }

    return true;
}
void CrossbarWeaponskillMacro::TriggerMacro()
{
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    if (strcmp(mMacroSettings.IconCommand, "DEFAULT") == 0)
    {
        char buffer[256];
        sprintf_s(buffer, 256, "/ws \"%s\" %s", pAbility->Name[0], (pAbility->Targets == 1) ? "<me>" : "<t>");
        pAshitaCore->GetChatManager()->QueueCommand(-1, buffer);
    }
    else
    {
        pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
    }
}