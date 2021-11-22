#include "CrossbarBindings.h"

SingleMacroInfo_t::SingleMacroInfo_t()
    : Type(IconType::None)
    , pResource(NULL)
    , DrawCost(DrawSetting::Default)
    , DrawCross(DrawSetting::Default)
    , DrawFade(DrawSetting::Default)
    , DrawName(DrawSetting::Default)
    , DrawRecast(DrawSetting::Default)
    , DrawSkillchain(DrawSetting::Default)
    , DrawTrigger(DrawSetting::Default)
{
    strcpy_s(IconCommand, 256, "DEFAULT");
    strcpy_s(IconText, 256, "DEFAULT");
    strcpy_s(IconFile, 256, "DEFAULT");
}
SingleMacroInfo_t::SingleMacroInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode)
    : Type(IconType::None)
    , pResource(NULL)
    , DrawCost(DrawSetting::Default)
    , DrawCross(DrawSetting::Default)
    , DrawFade(DrawSetting::Default)
    , DrawName(DrawSetting::Default)
    , DrawRecast(DrawSetting::Default)
    , DrawSkillchain(DrawSetting::Default)
    , DrawTrigger(DrawSetting::Default)
{
    strcpy_s(IconCommand, 256, "DEFAULT");
    strcpy_s(IconText, 256, "DEFAULT");
    strcpy_s(IconFile, 256, "DEFAULT");

    for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
    {
        if (_stricmp(subNode->name(), "type") == 0)
        {
            if (_stricmp(subNode->value(), "ability") == 0)
                Type = IconType::Ability;
            else if (_stricmp(subNode->value(), "spell") == 0)
                Type = IconType::Spell;
            else if (_stricmp(subNode->value(), "weaponskill") == 0)
                Type = IconType::Weaponskill;
            else if (_stricmp(subNode->value(), "item") == 0)
                Type = IconType::Item;
            else if (_stricmp(subNode->value(), "command") == 0)
                Type = IconType::Command;
            else
                Type = IconType::None;
        }
        else if (_stricmp(subNode->name(), "resource") == 0)
        {
            if (Type == IconType::Ability)
            {
                IAbility* pResource = NULL;
                if (XmlLoader::IsNumber(subNode->value()))
                    pResource = pAshitaCore->GetResourceManager()->GetAbilityById(atoi(subNode->value()) + 512);
                else
                    pResource = pAshitaCore->GetResourceManager()->GetAbilityByName(subNode->value(), 0);
                if (pResource != NULL)
                {
                    this->pResource = pResource;
                }
            }
            else if (Type == IconType::Spell)
            {
                ISpell* pResource = NULL;
                if (XmlLoader::IsNumber(subNode->value()))
                    pResource = pAshitaCore->GetResourceManager()->GetSpellById(atoi(subNode->value()));
                else
                    pResource = pAshitaCore->GetResourceManager()->GetSpellByName(subNode->value(), 0);
                if (pResource != NULL)
                {
                    this->pResource = pResource;
                }
            }
            else if (Type == IconType::Weaponskill)
            {
                IAbility* pResource = NULL;
                if (XmlLoader::IsNumber(subNode->value()))
                    pResource = pAshitaCore->GetResourceManager()->GetAbilityById(atoi(subNode->value()));
                else
                    pResource = pAshitaCore->GetResourceManager()->GetAbilityByName(subNode->value(), 0);
                if (pResource != NULL)
                {
                    this->pResource = pResource;
                }
            }
            else if (Type == IconType::Item)
            {
                IItem* pResource = NULL;
                if (XmlLoader::IsNumber(subNode->value()))
                    pResource = pAshitaCore->GetResourceManager()->GetItemById(atoi(subNode->value()));
                else
                    pResource = pAshitaCore->GetResourceManager()->GetItemByName(subNode->value(), 0);
                if (pResource != NULL)
                {
                    this->pResource = pResource;
                }
            }
        }
        else if (_stricmp(subNode->name(), "command") == 0)
        {
            strcpy_s(IconCommand, 256, subNode->value());
        }
        else if (_stricmp(subNode->name(), "text") == 0)
        {
            strcpy_s(IconText, 256, subNode->value());
        }
        else if (_stricmp(subNode->name(), "file") == 0)
        {
            sprintf_s(IconFile, 256, "%s", subNode->value());
        }
        else if (_stricmp(subNode->name(), "drawcost") == 0)
        {
            DrawCost = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
        else if (_stricmp(subNode->name(), "drawcross") == 0)
        {
            DrawCross = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
        else if (_stricmp(subNode->name(), "drawfade") == 0)
        {
            DrawFade = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
        else if (_stricmp(subNode->name(), "drawname") == 0)
        {
            DrawName = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
        else if (_stricmp(subNode->name(), "drawrecast") == 0)
        {
            DrawRecast = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
        else if (_stricmp(subNode->name(), "drawskillchain") == 0)
        {
            DrawSkillchain = (_stricmp(subNode->value(), "false") == 0) ? DrawSetting::DontDraw : DrawSetting::Draw;
        }
    }
}
void SingleMacroInfo_t::BooleanToTag(std::ofstream* stream, const char* field, int depth, DrawSetting setting)
{
    if (setting == DrawSetting::Default)
        return;

    *stream << std::string(depth * 4, ' ');
    *stream << "<" << field << ">";
    *stream << (setting == DrawSetting::Draw) ? "true" : "false";
    *stream << "</" << field << ">\n";
}
void SingleMacroInfo_t::Write(std::ofstream* stream, int depth)
{
    *stream << std::string(depth * 4, ' ') << "<type>";
    switch (Type)
    {
    case IconType::Ability:
        *stream << "Ability";
        break;
    case IconType::Spell:
        *stream << "Spell";
        break;
    case IconType::Weaponskill:
        *stream << "Weaponskill";
        break;
    case IconType::Item:
        *stream << "Item";
        break;
    case IconType::Command:
        *stream << "Command";
        break;
    }
    *stream << "</type>\n";

    if (pResource != NULL)
    {
        *stream << std::string(depth * 4, ' ') << "<resource>";
        switch (Type)
        {
        case IconType::Ability:
            *stream << ((IAbility*)pResource)->Id - 512 << "</resource> <!--";
            *stream << ((IAbility*)pResource)->Name[0] << "-->\n";
            break;
        case IconType::Spell:
            *stream << ((ISpell*)pResource)->Index << "</resource> <!--";
            *stream << ((ISpell*)pResource)->Name[0] << "-->\n";
            break;
        case IconType::Weaponskill:
            *stream << ((IAbility*)pResource)->Id << "</resource> <!--";
            *stream << ((IAbility*)pResource)->Name[0] << "-->\n";
            break;
        case IconType::Item:
            *stream << ((IItem*)pResource)->Id << "</resource> <!--";
            *stream << ((IItem*)pResource)->Name[0] << "-->\n";
            break;
        }
    }


    if (strcmp(IconCommand, "DEFAULT") != 0)
    {
        *stream << std::string(depth * 4, ' ') << "<command>" << XmlLoader::Escape(IconCommand) << "</command>\n";
    }
    if (strcmp(IconText, "DEFAULT") != 0)
    {
        *stream << std::string(depth * 4, ' ') << "<text>" << XmlLoader::Escape(IconText) << "</text>\n";
    }
    if (strcmp(IconFile, "DEFAULT") != 0)
    {
        *stream << std::string(depth * 4, ' ') << "<file>" << XmlLoader::Escape(IconFile) << "</file>\n";
    }
    BooleanToTag(stream, "drawcost", depth, DrawCost);
    BooleanToTag(stream, "drawcross", depth, DrawCross);
    BooleanToTag(stream, "drawfade", depth, DrawFade);
    BooleanToTag(stream, "drawname", depth, DrawName);
    BooleanToTag(stream, "drawrecast", depth, DrawRecast);
    BooleanToTag(stream, "drawskillchain", depth, DrawSkillchain);
    BooleanToTag(stream, "drawtrigger", depth, DrawTrigger);
}

SingleTriggerInfo_t::SingleTriggerInfo_t()
{}
SingleTriggerInfo_t::SingleTriggerInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode)
{
    for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
    {
        if (strcmp(subNode->name(), "dpadup") == 0)
        {
            Buttons[(int)MacroButton::DpadUp] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "dpadright") == 0)
        {
            Buttons[(int)MacroButton::DpadRight] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "dpaddown") == 0)
        {
            Buttons[(int)MacroButton::DpadDown] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "dpadleft") == 0)
        {
            Buttons[(int)MacroButton::DpadLeft] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "buttonup") == 0)
        {
            Buttons[(int)MacroButton::ButtonUp] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "buttonright") == 0)
        {
            Buttons[(int)MacroButton::ButtonRight] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "buttondown") == 0)
        {
            Buttons[(int)MacroButton::ButtonDown] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "buttonleft") == 0)
        {
            Buttons[(int)MacroButton::ButtonLeft] = SingleMacroInfo_t(pAshitaCore, pSettings, subNode);
        }
    }
}
const char* SingleTriggerInfo_t::GetButtonTag(int index)
{
    switch (index)
    {
    case 0:
        return "dpadup";
    case 1:
        return "dpadright";
    case 2:
        return "dpaddown";
    case 3:
        return "dpadleft";
    case 4:
        return "buttonup";
    case 5:
        return "buttonright";
    case 6:
        return "buttondown";
    case 7:
        return "buttonleft";
    }
    return "";
}
void SingleTriggerInfo_t::Write(std::ofstream* stream, int depth)
{
    for (int x = 0; x < 8; x++)
    {
        if (Buttons[x].Type == IconType::None)
            continue;

        const char* button = GetButtonTag(x);

        *stream << std::string(depth * 4, ' ');
        *stream << "<" << button << ">\n";
        Buttons[x].Write(stream, depth + 1);
        *stream << std::string(depth * 4, ' ');
        *stream << "</" << button << ">\n";
    }
}

SingleBindingInfo_t::SingleBindingInfo_t()
{
}
SingleBindingInfo_t::SingleBindingInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode)
{
    for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
    {
        if (strcmp(subNode->name(), "lefttrigger") == 0)
        {
            Triggers[(int)MacroMode::LeftTrigger] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "righttrigger") == 0)
        {
            Triggers[(int)MacroMode::RightTrigger] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "bothtriggerslt") == 0)
        {
            Triggers[(int)MacroMode::BothTriggersLT] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "bothtriggersrt") == 0)
        {
            Triggers[(int)MacroMode::BothTriggersRT] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "lefttriggerdt") == 0)
        {
            Triggers[(int)MacroMode::LeftTriggerDT] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (strcmp(subNode->name(), "righttriggerdt") == 0)
        {
            Triggers[(int)MacroMode::RightTriggerDT] = SingleTriggerInfo_t(pAshitaCore, pSettings, subNode);
        }
    }
}
void SingleBindingInfo_t::Write(std::ofstream* stream, int depth)
{
    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for left trigger only.-->\n";
    *stream << std::string(depth * 4, ' ') << "<lefttrigger>\n";
    Triggers[(int)MacroMode::LeftTrigger].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</lefttrigger>\n";
    *stream << std::string(depth * 4, ' ') << "\n";

    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for right trigger only.-->\n";
    *stream << std::string(depth * 4, ' ') << "<righttrigger>\n";
    Triggers[(int)MacroMode::RightTrigger].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</righttrigger>\n";
    *stream << std::string(depth * 4, ' ') << "\n";

    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for LT>RT, but also used for LT+RT if allowpriority is disabled.-->\n";
    *stream << std::string(depth * 4, ' ') << "<bothtriggerslt>\n";
    Triggers[(int)MacroMode::BothTriggersLT].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</bothtriggerslt>\n";
    *stream << std::string(depth * 4, ' ') << "\n";

    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for RT>LT, and will not be used if allowpriority is disabled.-->\n";
    *stream << std::string(depth * 4, ' ') << "<bothtriggersrt>\n";
    Triggers[(int)MacroMode::BothTriggersRT].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</bothtriggersrt>\n";
    *stream << std::string(depth * 4, ' ') << "\n";

    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for left trigger double tap and will not be used if allowdoubletap is disabled.-->\n";
    *stream << std::string(depth * 4, ' ') << "<lefttriggerdt>\n";
    Triggers[(int)MacroMode::LeftTriggerDT].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</lefttriggerdt>\n";
    *stream << std::string(depth * 4, ' ') << "\n";

    *stream << std::string(depth * 4, ' ') << "<!--These bindings are for right trigger double tap and will not be used if allowdoubletap is disabled.-->\n";
    *stream << std::string(depth * 4, ' ') << "<righttriggerdt>\n";
    Triggers[(int)MacroMode::RightTriggerDT].Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</righttriggerdt>\n";
    *stream << std::string(depth * 4, ' ') << "\n";
}

SinglePaletteInfo_t::SinglePaletteInfo_t()
{
    strcpy_s(Name, 256, "Default");
    pBindings = new SingleBindingInfo_t();
}
SinglePaletteInfo_t::SinglePaletteInfo_t(const char* name)
{
    strcpy_s(Name, 256, name);
    pBindings = new SingleBindingInfo_t();
}
SinglePaletteInfo_t::~SinglePaletteInfo_t()
{
    delete pBindings;
}
SinglePaletteInfo_t::SinglePaletteInfo_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode)
{
    xml_attribute<>* attr = baseNode->first_attribute("name");
    if (attr)
        strcpy_s(Name, 256, attr->value());
    else
        strcpy_s(Name, 256, "Default");

    pBindings = new SingleBindingInfo_t(pAshitaCore, pSettings, baseNode);
}

JobSettings_t::JobSettings_t(IAshitaCore* pAshitaCore, int job)
{
    pJobDefaults = new SingleBindingInfo_t();
    pJobName = pAshitaCore->GetResourceManager()->GetString("jobs.names_abbr", job);
    mPaletteList.push_back(new SinglePaletteInfo_t());
    sprintf_s(mPaletteList.front()->Name, 256, "%s_1", pJobName);
    mPaletteIterator = mPaletteList.begin();
}
JobSettings_t::JobSettings_t(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, xml_node<>* baseNode, int job)
{
    pJobDefaults = new SingleBindingInfo_t();
    pJobName = pAshitaCore->GetResourceManager()->GetString("jobs.names_abbr", job);
    for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
    {
        if (_stricmp(subNode->name(), "default") == 0)
        {
            delete pJobDefaults;
            pJobDefaults = new SingleBindingInfo_t(pAshitaCore, pSettings, subNode);
        }
        else if (_stricmp(subNode->name(), "palette") == 0)
        {
            mPaletteList.push_back(new SinglePaletteInfo_t(pAshitaCore, pSettings, subNode));
        }
    }
    if (mPaletteList.size() == 0)
    {
        mPaletteList.push_back(new SinglePaletteInfo_t());
        sprintf_s(mPaletteList.front()->Name, 256, "%s_1", pJobName);
    }
    mPaletteIterator = mPaletteList.begin();
}
JobSettings_t::~JobSettings_t()
{
    if (pJobDefaults)
    {
        delete pJobDefaults;
    }
    for (mPaletteIterator = mPaletteList.begin(); mPaletteIterator != mPaletteList.end(); mPaletteIterator++)
    {
        delete* mPaletteIterator;
    }
}
void JobSettings_t::Write(std::ofstream* stream, int depth)
{
    *stream << std::string(depth * 4, ' ') << "<default>\n";
    pJobDefaults->Write(stream, depth + 1);
    *stream << std::string(depth * 4, ' ') << "</default>\n";

    for (std::list<SinglePaletteInfo_t*>::iterator iter = mPaletteList.begin(); iter != mPaletteList.end(); iter++)
    {
        *stream << std::string(depth * 4, ' ') << "<palette name=\"" << XmlLoader::Escape((*iter)->Name) << "\">\n";
        (*iter)->pBindings->Write(stream, depth + 1);
        *stream << std::string(depth * 4, ' ') << "</palette>\n";
    }
}

CrossbarBindings::CrossbarBindings(IAshitaCore* pAshitaCore, const char* playerName, uint32_t playerId, int playerJob, CrossbarSettings* pSettings)
    : pAshitaCore(pAshitaCore)
    , pSettings(pSettings)
    , pDisplayPalette(NULL)
    , mRedraw(false)
{
    Load(playerName, playerId, playerJob);
}
CrossbarBindings::~CrossbarBindings()
{
    if (pGlobalDefaults)
    {
        delete pGlobalDefaults;
    }
    if (pJobSettings)
    {
        delete pJobSettings;
    }
}
void CrossbarBindings::Load(const char* playerName, uint32_t playerId, int playerJob)
{
    strcpy_s(mPlayerName, 256, playerName);
    mPlayerId = playerId;
    mPlayerJob = playerJob;
    pGlobalDefaults = new SingleBindingInfo_t();
    pJobSettings = new JobSettings_t(pAshitaCore, playerJob);

    char* fileBuffer;
    xml_document<>* xmlBuffer;
    char path[256];
    sprintf_s(path, 256, "%sconfig/crossbar/%s_%u/bindings/default.xml", pAshitaCore->GetInstallPath(), mPlayerName, mPlayerId);
    if (XmlLoader::Load(&fileBuffer, &xmlBuffer, path))
    {
        xml_node<>* firstNode = xmlBuffer->first_node();
        if (_stricmp(firstNode->name(), "crossbarbindings") == 0)
        {
            delete pGlobalDefaults;
            pGlobalDefaults = new SingleBindingInfo_t(pAshitaCore, pSettings, firstNode);
        }
        delete xmlBuffer;
        delete[] fileBuffer;
    }

    sprintf_s(path, 256, "%sconfig/crossbar/%s_%u/bindings/%s.xml", pAshitaCore->GetInstallPath(), mPlayerName, mPlayerId, pAshitaCore->GetResourceManager()->GetString("jobs.names_abbr", mPlayerJob));
    if (XmlLoader::Load(&fileBuffer, &xmlBuffer, path))
    {
        xml_node<>* firstNode = xmlBuffer->first_node();
        if (_stricmp(firstNode->name(), "crossbarbindings") == 0)
        {
            delete pJobSettings;
            pJobSettings = new JobSettings_t(pAshitaCore, pSettings, firstNode, playerJob);
        }
        delete xmlBuffer;
        delete[] fileBuffer;
    }
}
void CrossbarBindings::SaveDefaults()
{
    char path[256];
    sprintf_s(path, 256, "%sconfig/crossbar/%s_%u/bindings/default.xml", pAshitaCore->GetInstallPath(), mPlayerName, mPlayerId);
    XmlLoader::CreateDirectories(path);
    std::ofstream outStream(path);
    if (outStream.is_open())
    {
        outStream << "<crossbarbindings>\n";
        pGlobalDefaults->Write(&outStream, 1);
        outStream << "</crossbarbindings>";
        outStream.close();
    }
}
void CrossbarBindings::SaveJob()
{
    char path[256];
    sprintf_s(path, 256, "%sconfig/crossbar/%s_%u/bindings/%s.xml", pAshitaCore->GetInstallPath(), mPlayerName, mPlayerId, pJobSettings->pJobName);
    XmlLoader::CreateDirectories(path);
    std::ofstream outStream(path);
    if (outStream.is_open())
    {
        outStream << "<crossbarbindings>\n";
        pJobSettings->Write(&outStream, 1);
        outStream << "</crossbarbindings>";
        outStream.close();
    }
}