#include "CrossbarSelectBindFlags.h"

CrossbarSelectBindFlags::CrossbarSelectBindFlags(FontMenuBase* pMainMenu, SingleMacroInfo_t macro, const char* buttonName)
	: FontMenuBase(pMainMenu, GetInitialState(macro, buttonName))
	, mMacro(macro)
{
	strcpy_s(mButtonName, 256, buttonName);
}

void CrossbarSelectBindFlags::HandleConfirm()
{
    if (mIsHidden)
        return;

	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (pOption->GetType() == FontMenuOptionType::CycleOption)
	{
		pOption->Advance();
	}
	else if (pOption->GetType() == FontMenuOptionType::MenuOption)
	{
		FontMenuCompletionData_t data = pOption->GetReturn();
		if (strcmp(data.Result, "CANCEL") == 0)
		{
			mIsFinished = true;
			strcpy_s(mCompletionData.Result, 256, "CANCEL");
			return;
		}
		if (strcmp(data.Result, "CONFIRM") == 0)
		{
			ApplySettings();
			mIsFinished = true;
			strcpy_s(mCompletionData.Result, 256, "BINDING_CONFIRMED");
			mCompletionData.Data = malloc(sizeof(SingleMacroInfo_t));
			memcpy(mCompletionData.Data, &mMacro, sizeof(SingleMacroInfo_t));
			mCompletionData.DataSize = sizeof(SingleMacroInfo_t);
			return;
		}
	}
}

void CrossbarSelectBindFlags::HandleButtonUp()
{
    if (mIsHidden)
        return;

	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleButtonUp();
		return;
	}

	ApplySettings();
	mIsFinished = true;
	strcpy_s(mCompletionData.Result, 256, "BINDING_CONFIRMED");
	mCompletionData.Data = malloc(sizeof(SingleMacroInfo_t));
	memcpy(mCompletionData.Data, &mMacro, sizeof(SingleMacroInfo_t));
	mCompletionData.DataSize = sizeof(SingleMacroInfo_t);
	return;
}

FontMenuState CrossbarSelectBindFlags::GetInitialState(SingleMacroInfo_t macro, const char* buttonName)
{
	std::vector<std::string> values;
	values.push_back("Disabled");
	values.push_back("Enabled");
	values.push_back("Default");


	std::vector<FontMenuOption> options;
	options.push_back(FontMenuOption("Command", macro.IconCommand, true));
	options.push_back(FontMenuOption("Label", macro.IconText, true));
	options.push_back(FontMenuOption("Icon", macro.IconFile, true));

	FontMenuOption option("Display Cost", values);
	for (int x = 0; x < (int)macro.DrawCost; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Cross", values);
	for (int x = 0; x < (int)macro.DrawCross; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Fade", values);
	for (int x = 0; x < (int)macro.DrawFade; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Label", values);
	for (int x = 0; x < (int)macro.DrawName; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Recast", values);
	for (int x = 0; x < (int)macro.DrawRecast; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Skillchain", values);
	for (int x = 0; x < (int)macro.DrawSkillchain; x++)
		option.Advance();
	options.push_back(option);

	option = FontMenuOption("Display Trigger", values);
	for (int x = 0; x < (int)macro.DrawTrigger; x++)
		option.Advance();
	options.push_back(option);
	options.push_back(FontMenuOption("Confirm", "CONFIRM", false));

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonUp, "Confirm Binding"),
		FontMenuHint(MacroButton::ButtonLeft, "Type In Value"),
		FontMenuHint(MacroButton::Cancel, "Return To Action Select")
	};

	return FontMenuState(options, hints, buttonName);
}
void CrossbarSelectBindFlags::ApplySettings()
{
	strcpy_s(mMacro.IconCommand, 256, mState.mOptions[0].GetValue());
	strcpy_s(mMacro.IconText, 256, mState.mOptions[1].GetValue());
	if (strcmp(mState.mOptions[2].GetValue(), "DEFAULT") != 0)
	{
		sprintf_s(mMacro.IconFile, 256, "%sresources//crossbar//%s", pAshitaCore->GetInstallPath(), mState.mOptions[2].GetValue());
	}
	mMacro.DrawCost = GetDrawSetting(mState.mOptions[3].GetValue());
	mMacro.DrawCross = GetDrawSetting(mState.mOptions[4].GetValue());
	mMacro.DrawFade = GetDrawSetting(mState.mOptions[5].GetValue());
	mMacro.DrawName = GetDrawSetting(mState.mOptions[6].GetValue());
	mMacro.DrawRecast = GetDrawSetting(mState.mOptions[7].GetValue());
	mMacro.DrawSkillchain = GetDrawSetting(mState.mOptions[8].GetValue());
	mMacro.DrawTrigger = GetDrawSetting(mState.mOptions[9].GetValue());
}
DrawSetting CrossbarSelectBindFlags::GetDrawSetting(const char* text)
{
	if (strcmp(text, "Disabled") == 0)
		return DrawSetting::DontDraw;
	else if (strcmp(text, "Enabled") == 0)
		return DrawSetting::Draw;
	else
		return DrawSetting::Default;
}