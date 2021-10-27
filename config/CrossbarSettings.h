#ifndef __ASHITA_CrossbarSettings_H_INCLUDED__
#define __ASHITA_CrossbarSettings_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../Defines.h"

struct BindingMenuTheme_t
{
    Gdiplus::Image* pHint[12];
    GDITextElementInfo* pPrompt;
    GDITextElementInfo* pMenu;
    GDITextElementInfo* pLabel;

    int mMaxOptions;
    int mMaxLabels;
    int mMenuWidth;
    int mOptionHeight;
    int mLabelHeight;
    int mLabelImageOffsetX;
    int mLabelImageOffsetY;
    int mLabelImageWidth;
    int mLabelImageHeight;
    bool mPlaystationConfirm;

    BindingMenuTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode, bool playstationConfirm);
    ~BindingMenuTheme_t();
};

struct SubPanelTheme_t
{
    int PanelWidth;
    int PanelHeight;
    int PanelSpacing;

    int MacroWidth;
    int MacroHeight;
    int MacroOffsetX[8];
    int MacroOffsetY[8];
    int MacroPlacementX;
    int MacroPlacementY;

    int IconWidth;
    int IconHeight;
    int IconOffsetX;
    int IconOffsetY;

    int ButtonsOffsetX;
    int ButtonsOffsetY;
    int ButtonsWidth;
    int ButtonsHeight;

    int DpadOffsetX;
    int DpadOffsetY;
    int DpadWidth;
    int DpadHeight;

    Gdiplus::Image* pButtons;
    Gdiplus::Image* pDpad;
    Gdiplus::Image* pFrame;

    SubPanelTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode);
    ~SubPanelTheme_t();
};

enum class SkillchainIndex
{
    Liquefaction = 0,
    Scission = 1,
    Reverberation = 2,
    Detonation = 3,
    Induration = 4,
    Impaction = 5,
    Transfixion = 6,
    Compression = 7,
    Fusion = 8,
    Gravitation = 9,
    Distortion = 10,
    Fragmentation = 11,
    Light = 12,
    Darkness = 13,
    None
};

struct MacroTheme_t
{
    GDITextElementInfo* pCost;
    GDITextElementInfo* pName;
    GDITextElementInfo* pRecast;
    Gdiplus::SolidBrush* pTriggerBrush;
    Gdiplus::SolidBrush* pBackgroundBrush;
    Gdiplus::Image* pDefaultAbilityIcon;
    Gdiplus::Image* pDefaultCommandIcon;
    Gdiplus::Image* pDefaultItemIcon;
    Gdiplus::Image* pDefaultSpellIcon;
    Gdiplus::Image* pDefaultWeaponskillIcon;
    Gdiplus::Image* pSkillchain[14];
    Gdiplus::Image* pCrawl[7];

    MacroTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode);
    ~MacroTheme_t();
};

struct InputConfig_t
{
    int MenuDuration;
    int TapDuration;
    bool AllowDoubleTap;
    bool AllowPriority;
    bool AllowButtonsInMenu;
    bool EnableXInput;
    bool EnableDInput;

    InputConfig_t()
        : MenuDuration(1200)
        , TapDuration(600)
        , AllowDoubleTap(false)
        , AllowPriority(false)
        , AllowButtonsInMenu(false)
        , EnableXInput(true)
        , EnableDInput(true)
    {}
};

struct CrossbarConfig_t
{
    bool EnableItemResource;
    bool EnableCost;
    bool EnableCross;
    bool EnableFade;
    bool EnableName;
    bool EnableRecast;
    bool EnableSkillchain;
    bool EnableSkillchainAnimation;
    bool EnableTrigger;
    bool HideEmptyIcons;

    int FadeAlpha;
    int TriggerDuration;
    bool UsePlaystationConfirm;
    bool UseLevelSync;

    bool HideWhileZoning;
    bool HideWhileCutscene;
    bool HideWhileMap;

    int MainPanelX;
    int MainPanelY;
    int SubPanelX;
    int SubPanelY;
    int BindMenuX;
    int BindMenuY;

    CrossbarConfig_t()
        : EnableItemResource(true)
        , EnableCost(true)
        , EnableCross(true)
        , EnableFade(true)
        , EnableName(true)
        , EnableRecast(true)
        , EnableSkillchain(true)
        , EnableSkillchainAnimation(true)
        , EnableTrigger(true)
        , FadeAlpha(140)
        , HideEmptyIcons(false)
        , TriggerDuration(400)
        , UsePlaystationConfirm(false)
        , MainPanelX(-1)
        , MainPanelY(-150)
        , SubPanelX(-1)
        , SubPanelY(-150)
        , BindMenuX(-1)
        , BindMenuY(80)
        , UseLevelSync(false)
        , HideWhileZoning(true)
        , HideWhileCutscene(true)
        , HideWhileMap(true)
    {}
};

struct CrossbarSettings
{
    bool mIsLoaded;
    CrossbarConfig_t mConfig;
    InputConfig_t mInput;
    BindingMenuTheme_t* pBinding;
    MacroTheme_t* pMacro;
    SubPanelTheme_t* pSubPanel;

    CrossbarSettings(IAshitaCore* pAshitaCore, const char* playerName,  uint32_t playerId);
    ~CrossbarSettings();

    bool LoadTheme(IAshitaCore* pAshitaCore, const char* path);
    void WriteDefaultSettings(const char* path);
    static bool LoadImageFromResourceFolder(Gdiplus::Image** ppImage, IAshitaCore* pAshitaCore, const char* path);
};

#endif