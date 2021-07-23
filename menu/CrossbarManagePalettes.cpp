#include "CrossbarManagePalettes.h"

CrossbarManagePalettes::CrossbarManagePalettes(FontMenuBase* pMainMenu, CrossbarBindings* pBindings)
	: FontMenuBase(pMainMenu, GetInitialState(pBindings))
	, pBindings(pBindings)
{}
void CrossbarManagePalettes::HandleConfirm()
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
	if (strcmp(pOption->GetValue(), "CANCEL") == 0)
	{
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}
	int offset = atoi(pOption->GetValue());
	std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin();
	for (int x = 0; x < offset; x++)
		iter++;
	
	pBindings->pDisplayPalette = (*iter)->pBindings;
	pSubMenu = new CrossbarEditPalette(this, pBindings, *iter);
	pBindings->mRedraw = true;
	return;
}
void CrossbarManagePalettes::HandleButtonUp()
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

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (strcmp(pOption->GetValue(), "CANCEL") == 0)
		return;

	int offset = atoi(pOption->GetValue());
	if (offset == 0)
		return;

	else
	{
		SinglePaletteInfo_t* pCurrent = *pBindings->pJobSettings->mPaletteIterator;

		std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin();
		for (int x = 0; x < offset; x++)
			iter++;

		SinglePaletteInfo_t* moving = *iter;
		iter--;
		SinglePaletteInfo_t* swap = *iter;
		*iter = moving;
		iter++;
		*iter = swap;

		pBindings->pJobSettings->mPaletteIterator = pBindings->pJobSettings->mPaletteList.begin();
		while (*pBindings->pJobSettings->mPaletteIterator != pCurrent)
			pBindings->pJobSettings->mPaletteIterator++;

		pBindings->SaveJob();
		mState = GetInitialState(pBindings);
	}
}
void CrossbarManagePalettes::HandleButtonLeft()
{
    if (mIsHidden)
        return;
    if (mIsFinished)
        return;
    if (pSubMenu)
    {
        pSubMenu->HandleButtonLeft();
        return;
    }

	FontMenuOption* pOption = &mState.mOptions[mState.mSelectedIndex];
	if (strcmp(pOption->GetValue(), "CANCEL") == 0)
		return;

	int offset = atoi(pOption->GetValue());
	if (offset == (mState.mOptions.size() - 2))
		return;

	else
	{
		SinglePaletteInfo_t* pCurrent = *pBindings->pJobSettings->mPaletteIterator;

		std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin();
		for (int x = 0; x < offset; x++)
			iter++;

		SinglePaletteInfo_t* moving = *iter;
		iter++;
		SinglePaletteInfo_t* swap = *iter;
		*iter = moving;
		iter--;
		*iter = swap;

		pBindings->pJobSettings->mPaletteIterator = pBindings->pJobSettings->mPaletteList.begin();
		while (*pBindings->pJobSettings->mPaletteIterator != pCurrent)
			pBindings->pJobSettings->mPaletteIterator++;

		pBindings->SaveJob();
		mState = GetInitialState(pBindings);
	}
}
void CrossbarManagePalettes::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (strcmp(data.Result, "DELETE_PALETTE") == 0)
	{
		SinglePaletteInfo_t* currentPalette = *(pBindings->pJobSettings->mPaletteIterator);
		for (std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin(); iter != pBindings->pJobSettings->mPaletteList.end();)
		{
			if ((*iter)->pBindings == pBindings->pDisplayPalette)
			{
				if (*iter == currentPalette)
				{
					currentPalette = NULL;
				}
				delete* iter;
				iter = pBindings->pJobSettings->mPaletteList.erase(iter);
			}
			else
				iter++;
		}
		if (pBindings->pJobSettings->mPaletteList.size() == 0)
		{
			char nameBuffer[256];
			sprintf_s(nameBuffer, 256, "%s_1", pBindings->pJobSettings->pJobName);
			pBindings->pJobSettings->mPaletteList.push_back(new SinglePaletteInfo_t(nameBuffer));
		}
		if (currentPalette == NULL)
		{
			pBindings->pJobSettings->mPaletteIterator = pBindings->pJobSettings->mPaletteList.begin();
		}
	}
	pBindings->pDisplayPalette = NULL;
	pBindings->mRedraw = true;
	mState = GetInitialState(pBindings);
}

FontMenuState CrossbarManagePalettes::GetInitialState(CrossbarBindings* pBindings)
{
	std::vector<FontMenuOption> options;
	int offset = 0;
	for (std::list<SinglePaletteInfo_t*>::iterator iter = pBindings->pJobSettings->mPaletteList.begin(); iter != pBindings->pJobSettings->mPaletteList.end(); iter++)
	{
		options.push_back(FontMenuOption((*iter)->Name, std::to_string(offset).c_str(), false));
		offset++;
	}
	options.push_back(FontMenuOption("Cancel", "CANCEL", false));

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonUp, "Rearrange Upwards"),
		FontMenuHint(MacroButton::ButtonLeft, "Rearrange Downwards"),
		FontMenuHint(MacroButton::Cancel, "Return To Main Menu")
	};

	return FontMenuState(options, hints, "Palette Manager");
}