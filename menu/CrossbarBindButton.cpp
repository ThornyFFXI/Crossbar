#include "CrossbarBindButton.h"

CrossbarBindButton::CrossbarBindButton(FontMenuBase* pMainMenu, CrossbarBindings* pBindings)
	: FontMenuBase(pMainMenu, GetInitialState(pBindings))
	, pBindings(pBindings)
{

}
CrossbarBindButton::~CrossbarBindButton()
{

}

void CrossbarBindButton::HandleConfirm()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}

	FontMenuCompletionData_t data = mState.mOptions[mState.mSelectedIndex].GetReturn();

	if (strcmp(data.Result, "BIND_GLOBAL_DEFAULTS") == 0)
	{
		pBindings->pDisplayPalette = pBindings->pGlobalDefaults;
		pSubMenu = new CrossbarSelectBindButton(this, pBindings, pBindings->pDisplayPalette, true);
		pBindings->mRedraw = true;
		return;
	}
	if (strcmp(data.Result, "BIND_JOB_DEFAULTS") == 0)
	{
		pBindings->pDisplayPalette = pBindings->pJobSettings->pJobDefaults;
		pSubMenu = new CrossbarSelectBindButton(this, pBindings, pBindings->pDisplayPalette, false);
		pBindings->mRedraw = true;
		return;
	}
	if (strcmp(data.Result, "BIND_CURRENT_PALETTE") == 0)
	{
		pBindings->pDisplayPalette = (*pBindings->pJobSettings->mPaletteIterator)->pBindings;
		pSubMenu = new CrossbarSelectBindButton(this, pBindings, pBindings->pDisplayPalette, false);
		pBindings->mRedraw = true;
		return;
	}
	if (strcmp(data.Result, "CANCEL") == 0)
	{
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}
}

FontMenuState CrossbarBindButton::GetInitialState(CrossbarBindings* pBindings)
{
	std::vector<FontMenuOption> options = {
		FontMenuOption("Global Defaults", "BIND_GLOBAL_DEFAULTS", false),
		FontMenuOption("Job Defaults", "BIND_JOB_DEFAULTS", false),
		FontMenuOption("Current Palette", "BIND_CURRENT_PALETTE", false),
		FontMenuOption("Cancel", "CANCEL", false)
	};

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::Cancel, "Main Menu")
	};

	return FontMenuState(options, hints, "Select Palette To Bind");
}