#include "CrossbarAddPalette.h"

CrossbarAddPalette::CrossbarAddPalette(FontMenuBase* pMainMenu, CrossbarBindings* pBindings)
	: FontMenuBase(pMainMenu, GetInitialState(pBindings))
	, pBindings(pBindings)
{

}
CrossbarAddPalette::~CrossbarAddPalette()
{

}

void CrossbarAddPalette::HandleConfirm()
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

	FontMenuCompletionData_t data = mState.mOptions[mState.mSelectedIndex].GetReturn();

	if (strcmp(data.Result, "CONFIRM_PALETTE_ADD") == 0)
	{
		int offset = atoi(mState.mOptions[0].GetValue());
		std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin();
		for (int x = 1; x < offset; x++)
			iter++;

		pBindings->pJobSettings->mPaletteList.insert(iter, new SinglePaletteInfo_t(mState.mOptions[1].GetValue()));
		pBindings->SaveJob();
		
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "FINISH");
		return;
	}

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (pOption->GetType() == FontMenuOptionType::CycleOption)
	{
		pOption->Advance();
	}
	else if (pOption->GetType() == FontMenuOptionType::MenuOption)
	{
		mIsFinished = true;
		mCompletionData = pOption->GetReturn();
		return;
	}
}

FontMenuState CrossbarAddPalette::GetInitialState(CrossbarBindings* pBindings)
{
	int paletteCount = pBindings->pJobSettings->mPaletteList.size();
	std::vector<std::string> possiblePositions;
	for (int x = 0; x <= paletteCount; x++)
	{
		possiblePositions.push_back(std::to_string(x + 1));
	}
	FontMenuOption position("Position", possiblePositions);
	for (int x = 0; x < paletteCount; x++)
	{
		position.Advance();
	}

	char nameBuffer[256];
	sprintf_s(nameBuffer, 256, "%s_%d", pBindings->pJobSettings->pJobName, paletteCount + 1);

	std::vector<FontMenuOption> options = {
		position,
		FontMenuOption("Name", nameBuffer, true),
		FontMenuOption("Add", "CONFIRM_PALETTE_ADD", false),
		FontMenuOption("Cancel", "CANCEL", false)
	};
	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonLeft, "Enter Name"),
		FontMenuHint(MacroButton::Cancel, "Close Menu")
	};
	return FontMenuState(options, hints, "Add Palette");
}