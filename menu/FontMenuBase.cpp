#include "FontMenuBase.h"
#include <fstream>

FontMenuBase::FontMenuBase(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, FontMenuState state)
	: pAshitaCore(pAshitaCore)
	, mState(state)
	, pSettings(pSettings)
	, pSubMenu(NULL)
	, mIsSubMenu(false)
	, mIsFinished(false)
{
	pFontUI = new FontMenuUI(pAshitaCore, pSettings, "CrossbarFontUI");
}
FontMenuBase::FontMenuBase(FontMenuBase* pBase, FontMenuState state)
	: pAshitaCore(pBase->pAshitaCore)
	, pFontUI(pBase->pFontUI)
	, mState(state)
	, pSettings(pBase->pSettings)
	, pSubMenu(NULL)
	, mIsSubMenu(true)
	, mIsFinished(false)
{ }

FontMenuBase::~FontMenuBase()
{
	if (!mIsSubMenu)
	{
		delete pFontUI;
	}
}

void FontMenuBase::Draw()
{
	if (pSubMenu)
	{
		FontMenuCompletionData_t data;
		if (pSubMenu->GetIsFinished(&data))
		{
			delete pSubMenu;
			pSubMenu = NULL;
			HandleSubMenu(data);
		}
		else
		{
			pSubMenu->Draw();
			return;
		}
	}

	pFontUI->Draw(&mState);
}
bool FontMenuBase::GetIsFinished(FontMenuCompletionData_t* buffer)
{
	if (mIsFinished)
	{
		*buffer = mCompletionData;
		return true;
	}
	return false;
}
MacroMode FontMenuBase::GetModeOverride()
{
	if (pSubMenu)
	{
		return pSubMenu->GetModeOverride();
	}
	return MacroMode::NoTrigger;
}
void FontMenuBase::ReceiveText(const char* value)
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
	}
}
void FontMenuBase::HandleButtonUp()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleButtonUp();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}
	//No definition in default menu.
}
void FontMenuBase::HandleButtonRight()
{
	if (mIsFinished)
		return;
	if (pSettings->pBinding->mPlaystationConfirm)
		HandleConfirm();
	else
		HandleCancel();
}
void FontMenuBase::HandleButtonDown()
{
	if (mIsFinished)
		return;
	if (pSettings->pBinding->mPlaystationConfirm)
		HandleCancel();
	else
		HandleConfirm();
}
void FontMenuBase::HandleButtonLeft()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleButtonLeft();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (pOption->GetType() == FontMenuOptionType::InputOption)
	{
		mState.mAwaitingInput = true;
		swprintf_s(mState.mInputPrompt, 2048, L"Enter a value for: %S", pOption->GetLabel());
	}
}
void FontMenuBase::HandleConfirm()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleConfirm();
		return;
	}
	if (mState.mAwaitingInput)
	{
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
void FontMenuBase::HandleCancel()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleCancel();
		return;
	}
	if (mState.mAwaitingInput)
	{
		mState.mAwaitingInput = false;
		return;
	}

	mIsFinished = true;
	mCompletionData.DataSize = 0;
	strcpy_s(mCompletionData.Result, 256, "CANCEL");
}
void FontMenuBase::HandleDpadUp()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleDpadUp();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}

	mState.mSelectedIndex--;
	mState.mSelectedPosition--;
	if (mState.mSelectedIndex < 0)
	{
		mState.mSelectedIndex = mState.mOptionMax - 1;
		mState.mSelectedPosition = min(mState.mSelectedIndex, 9);
	}
	else if (mState.mSelectedPosition == 0)
	{
		if (mState.mSelectedIndex != 0)
			mState.mSelectedPosition = 1;
	}
}
void FontMenuBase::HandleDpadRight()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleDpadRight();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}

	//Unused in default menu.
}
void FontMenuBase::HandleDpadDown()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleDpadDown();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}

	mState.mSelectedIndex++;
	mState.mSelectedPosition++;
	if (mState.mSelectedIndex >= mState.mOptionMax)
	{
		mState.mSelectedIndex = 0;
		mState.mSelectedPosition = 0;
	}
	else if (mState.mSelectedPosition == 9)
	{
		if (mState.mSelectedIndex < (mState.mOptionMax - 1))
			mState.mSelectedPosition = 8;
	}
}
void FontMenuBase::HandleDpadLeft()
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleDpadLeft();
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}

	//Unused in default menu.
}
void FontMenuBase::HandleMacro(MacroButton button, MacroMode mode)
{
	if (mIsFinished)
		return;
	if (pSubMenu)
	{
		pSubMenu->HandleMacro(button, mode);
		return;
	}
	if (mState.mAwaitingInput)
	{
		return;
	}
	//Unused in default menu.
}
void FontMenuBase::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (data.DataSize != 0)
	{
		free(data.Data);
	}
}