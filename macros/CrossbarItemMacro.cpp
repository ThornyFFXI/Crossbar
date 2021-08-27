#include "CrossbarItemMacro.h"

CrossbarItemMacro::CrossbarItemMacro(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, SingleMacroInfo_t macroSettings, bool offset, int index)
    : CrossbarMacro(pAshitaCore, pSettings, macroSettings, offset, index)
{
    pGdiItem = NULL;
    pItem = reinterpret_cast<IItem*>(macroSettings.pResource);
    std::list<std::string> paths;
    char buffer[256];
    sprintf_s(buffer, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), macroSettings.IconFile);
    paths.push_back(std::string(buffer));
    if (pItem)
    {
        sprintf_s(buffer, 256, "%sresources//crossbar//items/%u.png", pAshitaCore->GetInstallPath(), pItem->Id);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//items/%u.bmp", pAshitaCore->GetInstallPath(), pItem->Id);
        paths.push_back(std::string(buffer));
        sprintf_s(buffer, 256, "%sresources//crossbar//items/%u.jpg", pAshitaCore->GetInstallPath(), pItem->Id);
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
        if (pSettings->mConfig.EnableItemResource)
        {
            LoadItemResource();
        }
        if (pImage == NULL)
        {
            pImage = pSettings->pMacro->pDefaultItemIcon;
        }
    }
}
CrossbarItemMacro::~CrossbarItemMacro()
{
    SAFE_DELETE(pGdiItem);
}

int CrossbarItemMacro::GetItemCount()
{
    IInventory* pInventory = pAshitaCore->GetMemoryManager()->GetInventory();
    int count              = 0;
    for (int x = 1; x <= pInventory->GetContainerCountMax(0); x++)
    {
        if (x == 81)
            break;

        Ashita::FFXI::item_t* item = pInventory->GetContainerItem(0, x);
        if ((item != NULL) && (item->Id == pItem->Id))
            count += item->Count;
    }
    for (int x = 1; x <= pInventory->GetContainerCountMax(3); x++)
    {
        if (x == 81)
            break;

        Ashita::FFXI::item_t* item = pInventory->GetContainerItem(3, x);
        if ((item != NULL) && (item->Id == pItem->Id))
            count += item->Count;
    }
    return count;
}
int CrossbarItemMacro::GetItemRecast()
{
    IInventory* pInventory = pAshitaCore->GetMemoryManager()->GetInventory();
    for (int x = 0; x < 16; x++)
    {
        Ashita::FFXI::equipmententry_t* pEquip = pInventory->GetEquippedItem(x);
        if (pEquip == NULL)
            continue;

        if (Read8(&pEquip->Index, 0) == 0)
            continue;

        int container = Read8(&pEquip->Index, 1);
        int index = Read8(&pEquip->Index, 0);

        Ashita::FFXI::item_t* item = pInventory->GetContainerItem(container, index);
        if (item->Id == pItem->Id)
        {
            if (item->Extra[3] & 0x40)
                return 0;

            uint32_t useTime   = Read32(item, 20) + mVanadielOffset;
            uint32_t timeStamp = GetRealTime();
            int32_t timeRemaining = useTime - timeStamp;

            //Calculate recast..
            return max(1, timeRemaining);
        }
    }
    return -1;
}

uint32_t CrossbarItemMacro::GetRealTime()
{
    DWORD realTime = Read32(pRealTime, 0);
    realTime       = Read32(realTime, 0);
    return Read32(realTime, 0x0C);
}

bool CrossbarItemMacro::Draw(GdiDIB* pDIB)
{
    bool redraw = mForceRedraw;
    mForceRedraw = false;
    
    bool isBlocked = false;
    bool isTransparent = false;
    if (pItem)
    {
        if (pItem->Flags & 0x800)
        {
            int recast = GetItemRecast();
            isTransparent = (recast > 0);
            if (recast == -1)
            {
                isBlocked = true;
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
        }
        else
        {
            int count = GetItemCount();
            if (count == 0)
            {
                isBlocked = true;
            }
            if (mMacroSettings.DrawRecast == DrawSetting::Draw)
            {
                if (count != mCount)
                {
                    redraw = true;
                }
            }
            mCount = count;
        }
    }

    if (mMacroSettings.DrawFade == DrawSetting::Draw)
    {
        if (isTransparent != mIsTransparent)
        {
            mIsTransparent = isTransparent;
            redraw = true;
        }
    }
    if (mMacroSettings.DrawCross == DrawSetting::Draw)
    {
        if (isBlocked != mIsBlocked)
        {
            mIsBlocked = isBlocked;
            redraw = true;
        }
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

    if (mMacroSettings.DrawRecast == DrawSetting::Draw)
    {
        if (pItem->Flags & 0x800)
        {
            if (mRecast != 0)
            {
                DrawRecast(pDIB, mRecast);
            }
        }
        else
        {
            DrawRecast(pDIB, mCount);
        }
    }

    if (mMacroSettings.DrawName == DrawSetting::Draw)
    {
        if (strcmp(mMacroSettings.IconText, "DEFAULT") == 0)
        {
            DrawName(pDIB, pItem->Name[0]);
        }
        else
        {
            DrawName(pDIB, mMacroSettings.IconText);
        }
    }

    return true;
}
void CrossbarItemMacro::LoadItemResource()
{
    pGdiItem = new GdiItem(pItem->Bitmap, pItem->ImageSize);
    pImage   = pGdiItem->GetImage();
}
void CrossbarItemMacro::TriggerMacro()
{
    mActivationTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(pSettings->mConfig.TriggerDuration);
    if (strcmp(mMacroSettings.IconCommand, "DEFAULT") == 0)
    {
        char buffer[256];
        sprintf_s(buffer, 256, "/item \"%s\" %s", pItem->Name[0], (pItem->Targets == 1) ? "<me>" : "<t>");
        pAshitaCore->GetChatManager()->QueueCommand(-1, buffer);
    }
    else
    {
        pAshitaCore->GetChatManager()->QueueCommand(-1, mMacroSettings.IconCommand);
    }
}