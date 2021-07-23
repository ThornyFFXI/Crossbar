#ifndef __ASHITA_CrossbarBindButton_H_INCLUDED__
#define __ASHITA_CrossbarBindButton_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "../config/CrossbarBindings.h"
#include "FontMenuBase.h"
#include "CrossbarSelectBindButton.h"

class CrossbarBindButton : public FontMenuBase
{
private:
    CrossbarBindings* pBindings;

public:
    CrossbarBindButton(FontMenuBase* pMainMenu, CrossbarBindings* pBindings);
    ~CrossbarBindButton();

private:
    void HandleConfirm() override;
    static FontMenuState GetInitialState(CrossbarBindings* pBindings);
};

#endif