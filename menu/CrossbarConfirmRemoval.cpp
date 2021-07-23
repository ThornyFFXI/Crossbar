#include "CrossbarConfirmRemoval.h"

CrossbarConfirmRemoval::CrossbarConfirmRemoval(FontMenuBase* pMainMenu, const char* confirmText, const char* returnValue)
	: FontMenuBase(pMainMenu, GetInitialState(confirmText, returnValue))
{}
void CrossbarConfirmRemoval::HandleConfirm()
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

	mIsFinished = true;
	mCompletionData = mState.mOptions[mState.mSelectedIndex].GetReturn();
	return;
}

FontMenuState CrossbarConfirmRemoval::GetInitialState(const char* confirmText, const char* returnValue)
{
	std::vector<FontMenuOption> options =
	{
		FontMenuOption(confirmText, returnValue, false),
		FontMenuOption("Cancel", "CANCEL", false)
	};

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::Cancel, "Return To Palette Editor")
	};

	char title[256];
	sprintf_s(title, 256, "Confirm Action: %s", confirmText);
	return FontMenuState(options, hints, title);
}