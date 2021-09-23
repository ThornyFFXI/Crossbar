#ifndef __ASHITA_CrossbarBindings_H_INCLUDED__
#define __ASHITA_CrossbarBindings_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../Defines.h"
#include "CrossbarSettings.h"
using namespace rapidxml;

enum class IconType
{
    Ability,
    Spell,
    Weaponskill,
    Item,
    Command,
    None
};
enum class DrawSetting
{
    DontDraw = 0,
    Draw     = 1,
    Default  = 2
};
struct SingleMacroInfo_t
{
    IconType Type;
    void* pResource;
    char IconCommand[256];
    char IconText[256];
    char IconFile[256];

    DrawSetting DrawCost;
    DrawSetting DrawCross;
    DrawSetting DrawFade;
    DrawSetting DrawName;
    DrawSetting DrawRecast;
    DrawSetting DrawSkillchain;
    DrawSetting DrawTrigger;

    SingleMacroInfo_t();
    SingleMacroInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode);
    void BooleanToTag(std::ofstream* stream, const char* field, int depth, DrawSetting setting);
    void Write(std::ofstream* stream, int depth);
};
struct SingleTriggerInfo_t
{
    SingleMacroInfo_t Buttons[8];

    SingleTriggerInfo_t();
    SingleTriggerInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode);
    const char* GetButtonTag(int index);
    void Write(std::ofstream* stream, int depth);
};
struct SingleBindingInfo_t
{
    SingleTriggerInfo_t Triggers[6];

    SingleBindingInfo_t();
    SingleBindingInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode);
    void Write(std::ofstream* stream, int depth);
};
struct SinglePaletteInfo_t
{
    char Name[256];
    SingleBindingInfo_t* pBindings;

    SinglePaletteInfo_t();
    SinglePaletteInfo_t(const char* name);
    ~SinglePaletteInfo_t();
    SinglePaletteInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode);
};
struct JobSettings_t
{
    const char* pJobName;
    SingleBindingInfo_t* pJobDefaults;
    std::list<SinglePaletteInfo_t*> mPaletteList;
    std::list<SinglePaletteInfo_t*>::iterator mPaletteIterator;

    JobSettings_t(IAshitaCore* pAshitaCore, int job);
    JobSettings_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode, int job);
    ~JobSettings_t();
    void Write(std::ofstream* stream, int depth);
};
class CrossbarBindings
{
private:
    IAshitaCore* pAshitaCore;
    CrossbarSettings* pSettings;
    char mPlayerName[256];
    uint32_t mPlayerId;
    int mPlayerJob;

public:
    SingleBindingInfo_t* pGlobalDefaults;
    JobSettings_t* pJobSettings;
    bool mRedraw;
    SingleBindingInfo_t* pDisplayPalette;

    CrossbarBindings(IAshitaCore* pAshitaCore, const char* playerName, uint32_t playerId, int playerJob, CrossbarSettings* psettings);
    ~CrossbarBindings();
    void Load(const char* playerName, uint32_t playerId, int playerJob);
    void SaveDefaults();
    void SaveJob();
};

#endif