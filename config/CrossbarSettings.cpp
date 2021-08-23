#include "CrossbarSettings.h"

BindingMenuTheme_t::BindingMenuTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode, bool playstationConfirm)
	: mPlaystationConfirm(playstationConfirm)
{
	for (int x = 0; x < 12; x++)
	{
		pHint[x] = NULL;
	}
	pPrompt = NULL;
	pMenu = NULL;
	pLabel = NULL;
	mMenuWidth = 400;

	for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
	{
		if (_stricmp(subNode->name(), "psconfirm") == 0)
			mPlaystationConfirm = (_stricmp(subNode->value(), "true") == 0);
		else if (_stricmp(subNode->name(), "maxoptions") == 0)
			mMaxOptions = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "maxlabels") == 0)
			mMaxLabels = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "menuwidth") == 0)
			mMenuWidth = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "optionheight") == 0)
			mOptionHeight = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "labelheight") == 0)
			mLabelHeight = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "labelimageoffsetx") == 0)
			mLabelImageOffsetX = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "labelimageoffsety") == 0)
			mLabelImageOffsetY = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "labelimagewidth") == 0)
			mLabelImageWidth = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "labelimageheight") == 0)
			mLabelImageHeight = atoi(subNode->value());

		else if (_stricmp(subNode->name(), "dpadup") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::DpadUp], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "dpadright") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::DpadRight], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "dpaddown") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::DpadDown], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "dpadleft") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::DpadLeft], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "buttonup") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::ButtonUp], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "buttonright") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::ButtonRight], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "buttondown") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::ButtonDown], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "buttonleft") == 0)
			CrossbarSettings::LoadImageFromResourceFolder(&pHint[(int)MacroButton::ButtonLeft], pAshitaCore, subNode->value());
		else if (_stricmp(subNode->name(), "prompt") == 0)
			pPrompt = new GDITextElementInfo(subNode);
		else if (_stricmp(subNode->name(), "menu") == 0)
			pMenu = new GDITextElementInfo(subNode);
		else if (_stricmp(subNode->name(), "label") == 0)
			pLabel = new GDITextElementInfo(subNode);
	}
	if (mPlaystationConfirm)
	{
		pHint[(int)MacroButton::Cancel] = pHint[(int)MacroButton::ButtonDown];
		pHint[(int)MacroButton::Confirm] = pHint[(int)MacroButton::ButtonRight];
	}
	else
	{
		pHint[(int)MacroButton::Cancel] = pHint[(int)MacroButton::ButtonRight];
		pHint[(int)MacroButton::Confirm] = pHint[(int)MacroButton::ButtonDown];
	}
}
BindingMenuTheme_t::~BindingMenuTheme_t()
{
	for (int x = 0; x < 10; x++)
	{
		SAFE_DELETE(pHint[x]);
	}
	SAFE_DELETE(pPrompt);
	SAFE_DELETE(pMenu);
	SAFE_DELETE(pLabel);
}

SubPanelTheme_t::SubPanelTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode)
{
	pButtons = NULL;
	pDpad = NULL;
	pFrame = NULL;
	for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
	{
		if (_stricmp(subNode->name(), "width") == 0)
			PanelWidth = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "height") == 0)
			PanelHeight = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "spacing") == 0)
			PanelSpacing = atoi(subNode->value());
		else if (_stricmp(subNode->name(), "macro") == 0)
		{
			for (xml_node<>* macroNode = subNode->first_node(); macroNode; macroNode = macroNode->next_sibling())
			{
				if (_stricmp(macroNode->name(), "width") == 0)
					MacroWidth = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "height") == 0)
					MacroHeight = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset0") == 0)
					MacroOffsetX[0] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset1") == 0)
					MacroOffsetX[1] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset2") == 0)
					MacroOffsetX[2] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset3") == 0)
					MacroOffsetX[3] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset4") == 0)
					MacroOffsetX[4] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset5") == 0)
					MacroOffsetX[5] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset6") == 0)
					MacroOffsetX[6] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "xoffset7") == 0)
					MacroOffsetX[7] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset0") == 0)
					MacroOffsetY[0] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset1") == 0)
					MacroOffsetY[1] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset2") == 0)
					MacroOffsetY[2] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset3") == 0)
					MacroOffsetY[3] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset4") == 0)
					MacroOffsetY[4] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset5") == 0)
					MacroOffsetY[5] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset6") == 0)
					MacroOffsetY[6] = atoi(macroNode->value());
				else if (_stricmp(macroNode->name(), "yoffset7") == 0)
					MacroOffsetY[7] = atoi(macroNode->value());

				if (_stricmp(macroNode->name(), "icon") == 0)
				{
					for (xml_node<>* iconNode = macroNode->first_node(); iconNode; iconNode = iconNode->next_sibling())
					{
						if (_stricmp(iconNode->name(), "frame") == 0)
							CrossbarSettings::LoadImageFromResourceFolder(&pFrame, pAshitaCore, iconNode->value());
						else if (_stricmp(iconNode->name(), "width") == 0)
							IconWidth = atoi(iconNode->value());
						else if (_stricmp(iconNode->name(), "height") == 0)
							IconHeight = atoi(iconNode->value());
						else if (_stricmp(iconNode->name(), "offsetx") == 0)
							IconOffsetX = atoi(iconNode->value());
						else if (_stricmp(iconNode->name(), "offsety") == 0)
							IconOffsetY = atoi(iconNode->value());
					}
				}
			}
		}
		else if (_stricmp(subNode->name(), "buttons") == 0)
		{
			for (xml_node<>* buttonNode = subNode->first_node(); buttonNode; buttonNode = buttonNode->next_sibling())
			{
				if (_stricmp(buttonNode->name(), "image") == 0)
					CrossbarSettings::LoadImageFromResourceFolder(&pButtons, pAshitaCore, buttonNode->value());
				else if (_stricmp(buttonNode->name(), "width") == 0)
					ButtonsWidth = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "height") == 0)
					ButtonsHeight = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "offsetx") == 0)
					ButtonsOffsetX = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "offsety") == 0)
					ButtonsOffsetY = atoi(buttonNode->value());
			}
		}
		else if (_stricmp(subNode->name(), "dpad") == 0)
		{
			for (xml_node<>* buttonNode = subNode->first_node(); buttonNode; buttonNode = buttonNode->next_sibling())
			{
				if (_stricmp(buttonNode->name(), "image") == 0)
					CrossbarSettings::LoadImageFromResourceFolder(&pDpad, pAshitaCore, buttonNode->value());
				else if (_stricmp(buttonNode->name(), "width") == 0)
					DpadWidth = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "height") == 0)
					DpadHeight = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "offsetx") == 0)
					DpadOffsetX = atoi(buttonNode->value());
				else if (_stricmp(buttonNode->name(), "offsety") == 0)
					DpadOffsetY = atoi(buttonNode->value());
			}
		}
	}
}
SubPanelTheme_t::~SubPanelTheme_t()
{
	SAFE_DELETE(pFrame);
	SAFE_DELETE(pButtons);
	SAFE_DELETE(pDpad);
}

MacroTheme_t::MacroTheme_t(IAshitaCore* pAshitaCore, xml_node<>* baseNode)
{
	pCost = NULL;
	pName = NULL;
	pRecast = NULL;
	pTriggerBrush = NULL;
	pBackgroundBrush = NULL;
	pDefaultAbilityIcon = NULL;
	pDefaultCommandIcon = NULL;
	pDefaultItemIcon = NULL;
	pDefaultSpellIcon = NULL;
	pDefaultWeaponskillIcon = NULL;
	for (int x = 0; x < 14; x++)
	{
		pSkillchain[x] = NULL;
	}
	for (int x = 0; x < 7; x++)
	{
		pCrawl[x] = NULL;
	}

	for (xml_node<>* subNode = baseNode->first_node(); subNode; subNode = subNode->next_sibling())
	{
		if (_stricmp(subNode->name(), "cost") == 0)
		{
			SAFE_DELETE(pCost);
			pCost = new GDITextElementInfo(subNode);
		}
		else if (_stricmp(subNode->name(), "recast") == 0)
		{
			SAFE_DELETE(pRecast);
			pRecast = new GDITextElementInfo(subNode);
		}
		else if (_stricmp(subNode->name(), "name") == 0)
		{
			SAFE_DELETE(pName);
			pName = new GDITextElementInfo(subNode);
		}
		else if (_stricmp(subNode->name(), "triggercolor") == 0)
		{
			SAFE_DELETE(pTriggerBrush);
			pTriggerBrush = new Gdiplus::SolidBrush(XmlLoader::ToColor(subNode));
		}
		else if (_stricmp(subNode->name(), "backgroundcolor") == 0)
		{
			SAFE_DELETE(pBackgroundBrush);
			pBackgroundBrush = new Gdiplus::SolidBrush(XmlLoader::ToColor(subNode));
		}
		else if (_stricmp(subNode->name(), "defaultability") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pDefaultAbilityIcon, pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "defaultcommand") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pDefaultCommandIcon, pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "defaultitem") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pDefaultItemIcon, pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "defaultspell") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pDefaultSpellIcon, pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "defaultweaponskill") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pDefaultWeaponskillIcon, pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "liquefaction") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[0], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "scission") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[1], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "reverberation") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[2], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "detonation") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[3], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "induration") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[4], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "impaction") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[5], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "transfixion") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[6], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "compression") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[7], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "fusion") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[8], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "gravitation") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[9], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "distortion") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[10], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "fragmentation") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[11], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "light") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[12], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "darkness") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pSkillchain[13], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl1") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[0], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl2") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[1], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl3") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[2], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl4") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[3], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl5") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[4], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl6") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[5], pAshitaCore, subNode->value());
		}
		else if (_stricmp(subNode->name(), "crawl7") == 0)
		{
			CrossbarSettings::LoadImageFromResourceFolder(&pCrawl[6], pAshitaCore, subNode->value());
		}
	}
}
MacroTheme_t::~MacroTheme_t()
{
	SAFE_DELETE(pCost);
	SAFE_DELETE(pName);
	SAFE_DELETE(pRecast);
	SAFE_DELETE(pTriggerBrush);
	SAFE_DELETE(pBackgroundBrush);
	SAFE_DELETE(pDefaultAbilityIcon);
	SAFE_DELETE(pDefaultCommandIcon);
	SAFE_DELETE(pDefaultItemIcon);
	SAFE_DELETE(pDefaultSpellIcon);
	SAFE_DELETE(pDefaultWeaponskillIcon);
	for (int x = 0; x < 14; x++)
	{
		SAFE_DELETE(pSkillchain[x]);
	}
	for (int x = 0; x < 7; x++)
	{
		SAFE_DELETE(pCrawl[x]);
	}
}

CrossbarSettings::CrossbarSettings(IAshitaCore* pAshitaCore, const char* playerName, uint32_t playerId)
{
	char themePath[256];
	sprintf_s(themePath, 256, "%sresources/crossbar/themes/xinput.xml", pAshitaCore->GetInstallPath());

	mConfig = CrossbarConfig_t();
	mInput = InputConfig_t();
	pBinding = NULL;
	pMacro = NULL;
	pSubPanel = NULL;

	char* fileBuffer;
	xml_document<>* xmlBuffer;
	char settingsPath[256];
	sprintf_s(settingsPath, 256, "%sconfig/crossbar/%s_%u/settings.xml", pAshitaCore->GetInstallPath(), playerName, playerId);
	if (!std::filesystem::exists(settingsPath))
	{
		XmlLoader::CreateDirectories(settingsPath);
		WriteDefaultSettings(settingsPath);
	}
	else if (XmlLoader::Load(&fileBuffer, &xmlBuffer, settingsPath))
	{
		xml_node<>* firstNode = xmlBuffer->first_node();
		if (_stricmp(firstNode->name(), "crossbarsettings") == 0)
		{
			for (xml_node<>* subNode = firstNode->first_node(); subNode; subNode = subNode->next_sibling())
			{
				if (_stricmp(subNode->name(), "theme") == 0)
					sprintf_s(themePath, 256, "%sresources/crossbar/themes/%s", pAshitaCore->GetInstallPath(), subNode->value());
				else if (_stricmp(subNode->name(), "allowpriority") == 0)
					mInput.AllowPriority = (_stricmp(subNode->value(), "true") == 0);
				else if (_stricmp(subNode->name(), "allowdoubletap") == 0)
					mInput.AllowDoubleTap = (_stricmp(subNode->value(), "true") == 0);
				else if (_stricmp(subNode->name(), "taptimeout") == 0)
					mInput.TapDuration = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "menuduration") == 0)
					mInput.MenuDuration = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "mainpanelx") == 0)
					mConfig.MainPanelX = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "mainpanely") == 0)
					mConfig.MainPanelY = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "subpanelx") == 0)
					mConfig.SubPanelX = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "subpanely") == 0)
					mConfig.SubPanelY = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "bindmenux") == 0)
					mConfig.BindMenuX = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "bindmenuy") == 0)
					mConfig.BindMenuY = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "enablecost") == 0)
					mConfig.EnableCost = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enablecross") == 0)
					mConfig.EnableCross = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enablefade") == 0)
					mConfig.EnableFade = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enablename") == 0)
					mConfig.EnableName = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enablerecast") == 0)
					mConfig.EnableRecast = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enableskillchain") == 0)
					mConfig.EnableSkillchain = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "enableskillchainanimation") == 0)
					mConfig.EnableSkillchainAnimation = (_stricmp(subNode->value(), "false") != 0);
				else if (_stricmp(subNode->name(), "fadealpha") == 0)
					mConfig.FadeAlpha = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "triggerduration") == 0)
					mConfig.TriggerDuration = atoi(subNode->value());
				else if (_stricmp(subNode->name(), "useplaystationconfirm") == 0)
                    mConfig.UsePlaystationConfirm = (_stricmp(subNode->value(), "false") != 0);
                else if (_stricmp(subNode->name(), "uselevelsync") == 0)
                    mConfig.UseLevelSync = (_stricmp(subNode->value(), "true") == 0);
                else if (_stricmp(subNode->name(), "allowbuttonsinmenu") == 0)
                    mInput.AllowButtonsInMenu = (_stricmp(subNode->value(), "true") == 0);
				else if (_stricmp(subNode->name(), "hideunboundbuttons") == 0)
					mConfig.HideEmptyIcons = (_stricmp(subNode->value(), "false") != 0);
			}
		}
		delete xmlBuffer;
		delete[] fileBuffer;
	}

	mIsLoaded = LoadTheme(pAshitaCore, themePath);
}
CrossbarSettings::~CrossbarSettings()
{
	if (pBinding)
	{
		delete pBinding;
	}
	if (pMacro)
	{
		delete pMacro;
	}
	if (pSubPanel)
	{
		delete pSubPanel;
	}
}

bool CrossbarSettings::LoadTheme(IAshitaCore* pAshitaCore, const char* path)
{
	char* fileBuffer;
	xml_document<>* xmlBuffer;
	if (!std::filesystem::exists(path))
	{
		return false;
	}
	else if (!XmlLoader::Load(&fileBuffer, &xmlBuffer, path))
	{
		return false;
	}

	xml_node<>* firstNode = xmlBuffer->first_node();
	if (_stricmp(firstNode->name(), "crossbartheme") == 0)
	{
		for (xml_node<>* subNode = firstNode->first_node(); subNode; subNode = subNode->next_sibling())
		{
			if (_stricmp(subNode->name(), "bindingmenu") == 0)
			{
				SAFE_DELETE(pBinding);
				pBinding = new BindingMenuTheme_t(pAshitaCore, subNode, mConfig.UsePlaystationConfirm);
			}
			else if (_stricmp(subNode->name(), "macro") == 0)
			{
				SAFE_DELETE(pMacro);
				pMacro = new MacroTheme_t(pAshitaCore, subNode);
			}
			else if (_stricmp(subNode->name(), "subpanel") == 0)
			{
				SAFE_DELETE(pSubPanel);
				pSubPanel = new SubPanelTheme_t(pAshitaCore, subNode);
			}
		}
	}
	delete xmlBuffer;
	delete[] fileBuffer;
	return ((pBinding != NULL) && (pMacro != NULL) && (pSubPanel != NULL));
}
bool CrossbarSettings::LoadImageFromResourceFolder(Gdiplus::Image** ppImage, IAshitaCore* pAshitaCore, const char* path)
{
	SAFE_DELETE(*ppImage);

	wchar_t textBuffer[1024];
	swprintf_s(textBuffer, 1024, L"%Sresources/crossbar/%S", pAshitaCore->GetInstallPath(), path);
	try
	{
		*ppImage = new Gdiplus::Image(textBuffer);
		return true;
	}
	catch (...)
	{
		*ppImage = NULL;
		pAshitaCore->GetChatManager()->Writef(0, false, "Failed to load image: %S", textBuffer);
	}
	return false;
}
void CrossbarSettings::WriteDefaultSettings(const char* path)
{
	std::ofstream outStream(path);
	if (outStream.is_open())
	{
		outStream << "<crossbarsettings>\n";
		outStream << "    <!--Themes should be in ashita/resources/crossbar/themes.-->\n";
		outStream << "    <theme>xinput.xml</theme>\n";
		outStream << "    \n";

		outStream << "    <!--If enabled, you will have seperate palettes for L2>R2 and R2>L2 based on the order buttons were depressed.  This adds 8 bindings.-->\n";
		outStream << "    <allowpriority>false</allowpriority>\n";
		outStream << "    \n";

		outStream << "    <!--If enabled, hitting L2 or R2 twice and holding it the second time will open a secondary palette.  This adds 16 bindings.-->\n";
		outStream << "    <allowdoubletap>false</allowdoubletap>\n";
		outStream << "    \n";

		outStream << "    <!--The length of time, in milliseconds, you are allotted to second-tap L2 or R2 when allowdoubletap is enabled.-->\n";
		outStream << "    <taptimeout>300</taptimeout>\n";
		outStream << "    \n";

		outStream << "    <!--The length of time, in milliseconds, you have to hold L1+L2+R1+R2 to open/close binding menu.-->\n";
		outStream << "    <menuduration>1200</menuduration>\n";
		outStream << "    \n";


		outStream << "    <!--Location for single trigger panel.  -1 in x will center.  Negative value in y will leave that value as a gap at bottom.-->\n";
		outStream << "    <mainpanelx>-1</mainpanelx>\n";
		outStream << "    <mainpanely>-150</mainpanely>\n";
		outStream << "    \n";
		outStream << "    <!--Location for dual trigger panel.  -1 in x will center.  Negative value in y will leave that value as a gap at bottom.-->\n";
		outStream << "    <subpanelx>-1</subpanelx>\n";
		outStream << "    <subpanely>-150</subpanely>\n";
		outStream << "    \n";
		outStream << "    <!--Location for menu.  -1 in x will center.  Negative value in y will leave that value as a gap at bottom.-->\n";
		outStream << "    <bindmenux>-1</bindmenux>\n";
		outStream << "    <bindmenuy>80</bindmenuy>\n";
		outStream << "    \n";
		outStream << "    <!--Enable or disable components of crossbar macros.-->\n";
		outStream << "    <enablecost>true</enablecost>\n";
		outStream << "    <enablecross>true</enablecross>\n";
		outStream << "    <enablefade>true</enablefade>\n";
		outStream << "    <enablename>true</enablename>\n";
		outStream << "    <enablerecast>true</enablerecast>\n";
		outStream << "    <enableskillchain>true</enableskillchain>\n";
		outStream << "    <enableskillchainanimation>true</enableskillchainanimation>\n";
		outStream << "    <enabletrigger>true</enabletrigger>\n";
		outStream << "    \n";
		outStream << "    <!--Alpha setting to be used when crossbar macros are faded.-->\n";
		outStream << "    <fadealpha>140</fadealpha>\n";
		outStream << "    \n";
		outStream << "    <!--Duration in milliseconds to flash crossbar macros when triggered.-->\n";
		outStream << "    <triggerduration>400</triggerduration>\n";
		outStream << "    \n";
		outStream << "    <!--If enabled, will swap bottom and right buttons in binding menu.-->\n";
        outStream << "    <useplaystationconfirm>false</useplaystationconfirm>\n";
        outStream << "    \n";
        outStream << "    <!--If enabled, binding menu will populate actions based on your synced level instead of real level.-->\n";
        outStream << "    <uselevelsync>false</uselevelsync>\n";
        outStream << "    \n";
        outStream << "    <!--If enabled, controller input will be sent directly to game while game has a menu open.-->\n";
        outStream << "    <allowbuttonsinmenu>false</allowbuttonsinmenu>\n";
		outStream << "    \n";
		outStream << "    <!--If enabled, will not render unbound button panels.-->\n";
		outStream << "    <hideunboundbuttons>false</hideunboundbuttons>\n";
		outStream << "</crossbarsettings>";
		outStream.close();
	}
}