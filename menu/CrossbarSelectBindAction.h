#ifndef __ASHITA_CrossbarSelectBindAction_H_INCLUDED__
#define __ASHITA_CrossbarSelectBindAction_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarSelectBindFlags.h"

enum class CrossbarActionType
{
    Command,
    Spell,
    Ability,
    Weaponskill,
    Item,
    Empty
};

class CrossbarSelectBindAction : public FontMenuBase
{
private:
    CrossbarActionType mType;
    int mSkill;
    char mButtonName[256];

public:
    CrossbarSelectBindAction(FontMenuBase* pMainMenu, CrossbarActionType type, const char* buttonName);
    CrossbarSelectBindAction(FontMenuBase* pMainMenu, CrossbarActionType type, int skill, const char* buttonName);

private:

    void HandleConfirm() override;
    void HandleButtonUp() override;
    static FontMenuState GetInitialState(IAshitaCore* pAshitaCore, CrossbarActionType type, int skill, const char* buttonName, bool useSync);

    void HandleSubMenu(FontMenuCompletionData_t data) override;
    void HandleButtonLeft() override;
    void ReceiveText(const char* value) override;
};

#endif