#include "CrossbarSelectBindButton.h"

CrossbarSelectBindButton::CrossbarSelectBindButton(FontMenuBase* pMainMenu, CrossbarBindings* pBindings, SingleBindingInfo_t* pPalette, bool global)
	: FontMenuBase(pMainMenu, GetInitialState(pBindings))
	, pBindings(pBindings)
	, pPalette(pPalette)
	, mGlobal(global)
{}

void CrossbarSelectBindButton::HandleCancel()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleCancel();
		return;
	}

	if (pBindings->pDisplayPalette)
	{
		pBindings->pDisplayPalette = NULL;
		pBindings->mRedraw = true;
	}
	mIsFinished = true;
	strcpy_s(mCompletionData.Result, 256, "CANCEL");
}

void CrossbarSelectBindButton::HandleConfirm()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}

	FontMenuCompletionData_t data = mState.mOptions[mState.mSelectedIndex].GetReturn();
	if (strcmp(data.Result, "CANCEL") == 0)
	{
		if (pBindings->pDisplayPalette)
		{
			pBindings->pDisplayPalette = NULL;
			pBindings->mRedraw = true;
		}
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}
}

void CrossbarSelectBindButton::HandleMacro(MacroButton button, MacroMode mode)
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleMacro(button, mode);
		return;
	}

	std::map<MacroButton, std::string> buttonNames = {
		{MacroButton::DpadUp, "Dpad-Up" },
		{MacroButton::DpadRight, "Dpad-Right" },
		{MacroButton::DpadDown, "Dpad-Down" },
		{MacroButton::DpadLeft, "Dpad-Left" },
		{MacroButton::ButtonUp, "Top Button" },
		{MacroButton::ButtonRight, "Right Button" },
		{MacroButton::ButtonDown, "Bottom Button" },
		{MacroButton::ButtonLeft, "Left Button" } };
	std::map<MacroMode, std::string> modeNames = {
		{MacroMode::LeftTrigger, "LT + "},
		{MacroMode::RightTrigger, "RT + "},
		{MacroMode::BothTriggersLT, "LT > RT + "},
		{MacroMode::BothTriggersRT, "RT > LT + "},
		{MacroMode::LeftTriggerDT, "LT(double) + "},
		{MacroMode::RightTriggerDT, "RT(double) + "}
	};

	std::map<MacroButton, std::string>::iterator iter = buttonNames.find(button);
	std::map<MacroMode, std::string>::iterator mIter = modeNames.find(mode);

	if ((iter != buttonNames.end()) && (mIter != modeNames.end()))
	{
		char buffer[256];
		sprintf_s(buffer, 256, "[%s%s]", mIter->second.c_str(), iter->second.c_str());
		pSubMenu = new CrossbarSelectBindType(this, &(pPalette->Triggers[(int)mode].Buttons[(int)button]), buffer, mode);
	}
}

FontMenuState CrossbarSelectBindButton::GetInitialState(CrossbarBindings* pBindings)
{
	std::vector<FontMenuOption> options = {
		FontMenuOption("Cancel", "CANCEL", false)
	};
	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::Cancel, "Return To Palette Select")
	};


	return FontMenuState(options, hints, "Press Button Combo To Bind");
}

void CrossbarSelectBindButton::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (strcmp(data.Result, "BINDING_CHANGED") == 0)
	{
		pBindings->mRedraw = true;
		if (mGlobal)
		{
			pBindings->SaveDefaults();
		}
		else
		{
			pBindings->SaveJob();
		}
	}
	if (data.DataSize != 0)
	{
		free(data.Data);
	}
}