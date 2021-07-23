#include "CrossbarMainMenu.h"

CrossbarMainMenu::CrossbarMainMenu(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings)
	: FontMenuBase(pAshitaCore, pSettings, GetInitialState())
	, pBindings(pBindings)
{

}
CrossbarMainMenu::~CrossbarMainMenu()
{

}

void CrossbarMainMenu::HandleConfirm()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}

	FontMenuCompletionData_t data = mState.mOptions[mState.mSelectedIndex].GetReturn();

	if (strcmp(data.Result, "OPEN_BIND_MENU") == 0)
	{
		pSubMenu = new CrossbarBindButton(this, pBindings);
	}
	else if (strcmp(data.Result, "OPEN_ADD_PALETTE_MENU") == 0)
	{
		pSubMenu = new CrossbarAddPalette(this, pBindings);
	}
	else if (strcmp(data.Result, "OPEN_MANAGE_PALETTE_MENU") == 0)
	{
		pSubMenu = new CrossbarManagePalettes(this, pBindings);
	}
	else
	{
		mIsFinished = true;
		mCompletionData = data;
	}
}

FontMenuState CrossbarMainMenu::GetInitialState()
{	
	std::vector<FontMenuOption> options = {
			FontMenuOption("Bind Buttons", "OPEN_BIND_MENU", false),
			FontMenuOption("Add Palette", "OPEN_ADD_PALETTE_MENU", false),
			FontMenuOption("Manage Palettes", "OPEN_MANAGE_PALETTE_MENU", false),
			FontMenuOption("CANCEL", "CLOSE_MENU", false)
	};
	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::Cancel, "Close Menu")
	};
	return FontMenuState(options, hints, "Main Menu");
}

void CrossbarMainMenu::HandleSubMenu(FontMenuCompletionData_t data)
{
}