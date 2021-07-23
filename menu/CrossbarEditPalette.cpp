#include "CrossbarEditPalette.h"

CrossbarEditPalette::CrossbarEditPalette(FontMenuBase* pMainMenu, CrossbarBindings* pBindings, SinglePaletteInfo_t* pPalette)
	: FontMenuBase(pMainMenu, GetInitialState(pPalette))
	, pBindings(pBindings)
	, pPalette(pPalette)
{}
void CrossbarEditPalette::HandleConfirm()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (strcmp(pOption->GetValue(), "CANCEL") == 0)
	{
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}
	if (strcmp(pOption->GetValue(), "CLEAR_BINDINGS") == 0)
	{
		char buffer[256];
		sprintf_s(buffer, 256, "Clear Bindings(%s)", pPalette->Name);
		pSubMenu = new CrossbarConfirmRemoval(this, buffer, "CONFIRM_CLEAR_BINDINGS");
		return;
	}
	if (strcmp(pOption->GetValue(), "DELETE_PALETTE") == 0)
	{
		char buffer[256];
		sprintf_s(buffer, 256, "Delete Palette(%s)", pPalette->Name);
		pSubMenu = new CrossbarConfirmRemoval(this, buffer, "CONFIRM_DELETE_PALETTE");
		return;
	}
}
void CrossbarEditPalette::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (strcmp(data.Result, "CONFIRM_CLEAR_BINDINGS") == 0)
	{
		strcpy_s(mCompletionData.Result, "CLEAR_BINDINGS");
		mIsFinished = true;
		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				pPalette->pBindings->Triggers[x].Buttons[y] = SingleMacroInfo_t();
			}
		}
		return;
	}
	if (strcmp(data.Result, "CONFIRM_DELETE_PALETTE") == 0)
	{
		strcpy_s(mCompletionData.Result, "DELETE_PALETTE");
		mIsFinished = true;
		return;
	}
}

void CrossbarEditPalette::ReceiveText(const char* value)
{
	if (pSubMenu)
	{
		pSubMenu->ReceiveText(value);
		return;
	}
	if (mState.mAwaitingInput)
	{
		FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
		pOption->SetReturn(value);
		mState.mAwaitingInput = false;
		strcpy_s(pPalette->Name, 256, value);
		pBindings->SaveJob();
		mState = GetInitialState(pPalette);
	}
}

FontMenuState CrossbarEditPalette::GetInitialState(SinglePaletteInfo_t* pPalette)
{
	std::vector<FontMenuOption> options =
	{
		FontMenuOption("Palette Name", pPalette->Name, true),
		FontMenuOption("Clear Bindings", "CLEAR_BINDINGS", false),
		FontMenuOption("Delete Palette", "DELETE_PALETTE", false),
		FontMenuOption("Cancel", "CANCEL", false)
	};

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonLeft, "Change Name"),
		FontMenuHint(MacroButton::Cancel, "Return To Palette Manager")
	};

	char title[256];
	sprintf_s(title, 256, "Palette: %s", pPalette->Name);
	return FontMenuState(options, hints, title);
}