#include "FontMenuUI.h"

FontMenuUI::FontMenuUI(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, const char* name)
	: pAshitaCore(pAshitaCore)
	, pSettings(pSettings)
{
	pPrimitiveObject = pAshitaCore->GetPrimitiveManager()->Create(name);
	pPrimitiveObject->SetVisible(false);
	int width = pSettings->pBinding->mMenuWidth;
	int height = 20 + (pSettings->pBinding->mOptionHeight * pSettings->pBinding->mMaxOptions) + (pSettings->pBinding->mLabelHeight * pSettings->pBinding->mMaxLabels) + (pSettings->pBinding->pPrompt->OffsetY + pSettings->pBinding->pPrompt->Height);
	pDIB = new GdiDIB(pAshitaCore->GetDirect3DDevice(), width, height);
	pDIB->GetGraphics()->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	pDIB->GetGraphics()->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
	pDIB->GetGraphics()->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);

	RECT rect;
	GetWindowRect(pAshitaCore->GetProperties()->GetFinalFantasyHwnd(), &rect);
	if (pSettings->mConfig.BindMenuX == -1)
	{
		pPrimitiveObject->SetPositionX((rect.right - rect.left - pSettings->pBinding->mMenuWidth) / 2);
	}
	else
	{
		pPrimitiveObject->SetPositionX(pSettings->mConfig.BindMenuX);
	}
	if (pSettings->mConfig.BindMenuY < 0)
	{
		pPrimitiveObject->SetPositionY((rect.bottom - rect.top) + pSettings->mConfig.BindMenuY - height);
	}
	else
	{
		pPrimitiveObject->SetPositionY(pSettings->mConfig.BindMenuY);
	}
}
FontMenuUI::~FontMenuUI()
{
	delete pDIB;
	pAshitaCore->GetPrimitiveManager()->Delete(pPrimitiveObject->GetAlias());
}

void FontMenuUI::Draw(FontMenuState* pState)
{
	pDIB->GetGraphics()->Clear(Gdiplus::Color::Black);
	int height = 0;
	if (pState->mAwaitingInput)
	{
		if (pSettings->pBinding->pPrompt == NULL)
		{
			height = 30;
		}
		else
		{
			DrawInputPrompt(pState);
			DrawOption(0, "To enter data, type /cbm [data] in console.", NULL);
			DrawOption(1, "Do not type the [] brackets.", NULL);
			height = (2 * pSettings->pBinding->mOptionHeight) + (pSettings->pBinding->pPrompt->OffsetY + pSettings->pBinding->pPrompt->Height) + 10 + pSettings->pBinding->pMenu->OffsetY;
		}
	}
	else
	{
		DrawPrompt(pState);
		DrawOptions(pState);
		height = DrawLabels(pState);
	}
	pDIB->ApplySectionToPrimitive(pPrimitiveObject, 0, 0, pSettings->pBinding->mMenuWidth, height);
	pPrimitiveObject->SetVisible(true);
}
void FontMenuUI::Hide()
{
    pPrimitiveObject->SetVisible(false);
}
void FontMenuUI::DrawInputPrompt(FontMenuState* pState)
{
	Gdiplus::Graphics* pGraphics = pDIB->GetGraphics();
	Gdiplus::Rect textRect(pSettings->pBinding->pPrompt->OffsetX, pSettings->pBinding->pPrompt->OffsetY, pSettings->pBinding->pPrompt->Width, pSettings->pBinding->pPrompt->Height);
	Gdiplus::GraphicsPath textPath;
	pSettings->pBinding->pPrompt->AddToPath(&textPath, pState->mInputPrompt, textRect);
	pGraphics->DrawPath(pSettings->pBinding->pPrompt->pFont->pPen, &textPath);
	pGraphics->FillPath(pSettings->pBinding->pPrompt->pFont->pBrush, &textPath);
}
void FontMenuUI::DrawOptions(FontMenuState* pState)
{
	int baseOption = max(0, (pState->mSelectedIndex - pState->mSelectedPosition));
	int drawIndex = 0;
	if (baseOption != 0)
	{
		DrawOption(drawIndex, "[...]", NULL);
		drawIndex++;
	}
	while (drawIndex < pSettings->pBinding->mMaxOptions)
	{
		int currOption = baseOption + drawIndex;
		if (currOption >= pState->mOptionMax)
			break;

		if ((drawIndex == (pSettings->pBinding->mMaxOptions - 1)) && (currOption < (pState->mOptionMax - 1)))
		{
			DrawOption(drawIndex, "[...]", NULL);
		}
		else
		{
			FontMenuOption* pOption = &pState->mOptions[currOption];
			DrawOption(drawIndex, pOption->GetLabel(), pOption->GetType() == FontMenuOptionType::MenuOption ? NULL : pOption->GetValue());
		}
		drawIndex++;
	}

	int offsetX = pSettings->pBinding->pMenu->OffsetX;
	int offsetY = (pState->mSelectedPosition * pSettings->pBinding->mOptionHeight) + (pSettings->pBinding->pPrompt->OffsetY + pSettings->pBinding->pPrompt->Height) + 10 + pSettings->pBinding->pMenu->OffsetY;
	int width = pSettings->pBinding->pMenu->Width;
	int height = pSettings->pBinding->pMenu->Height;
	Gdiplus::SolidBrush brush(Gdiplus::Color(80, 255, 255, 0));
	pDIB->GetGraphics()->FillRectangle(&brush, Gdiplus::Rect(offsetX, offsetY, width, height));
}
void FontMenuUI::DrawOption(int index, const char* label, const char* value)
{
	Gdiplus::Graphics* pGraphics = pDIB->GetGraphics();
	
	int offsetX = pSettings->pBinding->pMenu->OffsetX;
	int offsetY = (index * pSettings->pBinding->mOptionHeight) + (pSettings->pBinding->pPrompt->OffsetY + pSettings->pBinding->pPrompt->Height) + 10 + pSettings->pBinding->pMenu->OffsetY;
	
	wchar_t textBuffer[1024];
	swprintf_s(textBuffer, 1024, L"%S", label);
	pSettings->pBinding->pMenu->pFont->pFormat->SetAlignment(Gdiplus::StringAlignmentNear);
	Gdiplus::Rect textRect(offsetX, offsetY, pSettings->pBinding->pMenu->Width, pSettings->pBinding->pMenu->Height);
	Gdiplus::GraphicsPath textPath;
	pSettings->pBinding->pMenu->AddToPath(&textPath, textBuffer, textRect);
	if ((value != NULL) && (strlen(value) > 0))
	{
		swprintf_s(textBuffer, 1024, L"%S", value);
		pSettings->pBinding->pMenu->pFont->pFormat->SetAlignment(Gdiplus::StringAlignmentFar);
		textPath.CloseFigure();
		pSettings->pBinding->pMenu->AddToPath(&textPath, textBuffer, textRect);
	}
	pGraphics->FillPath(pSettings->pBinding->pMenu->pFont->pBrush, &textPath);
}
void FontMenuUI::DrawPrompt(FontMenuState* pState)
{
	Gdiplus::Graphics* pGraphics = pDIB->GetGraphics();
	Gdiplus::Rect textRect(pSettings->pBinding->pPrompt->OffsetX, pSettings->pBinding->pPrompt->OffsetY, pSettings->pBinding->pPrompt->Width, pSettings->pBinding->pPrompt->Height);
	Gdiplus::GraphicsPath textPath;
	pSettings->pBinding->pPrompt->AddToPath(&textPath, pState->mHelpText, textRect);
	pGraphics->DrawPath(pSettings->pBinding->pPrompt->pFont->pPen, &textPath);
	pGraphics->FillPath(pSettings->pBinding->pPrompt->pFont->pBrush, &textPath);
}
int FontMenuUI::DrawLabels(FontMenuState* pState)
{
	Gdiplus::Graphics* pGraphics = pDIB->GetGraphics();

	int offsetX = 0;
	int numOptions = min(pState->mOptionMax, pSettings->pBinding->mMaxOptions);
	int offsetY = (numOptions * pSettings->pBinding->mOptionHeight) + (pSettings->pBinding->pPrompt->OffsetY + pSettings->pBinding->pPrompt->Height) + 10 + pSettings->pBinding->pMenu->OffsetY;

	for (int x = 0; x < pState->mHintMax; x++)
	{
		if (x >= pSettings->pBinding->mMaxLabels)
			break;

		FontMenuHint* pHint = &pState->mHints[x];
		Gdiplus::Image* pImage = pSettings->pBinding->pHint[(int)pHint->mButton];
		if (pImage)
		{
			Gdiplus::Rect imageRect(offsetX + pSettings->pBinding->mLabelImageOffsetX, offsetY + pSettings->pBinding->mLabelImageOffsetY, pSettings->pBinding->mLabelImageWidth, pSettings->pBinding->mLabelImageHeight);
			pDIB->GetGraphics()->DrawImage(pImage, imageRect);
		}
		Gdiplus::Rect textRect(offsetX + pSettings->pBinding->pLabel->OffsetX, offsetY + pSettings->pBinding->pLabel->OffsetY, pSettings->pBinding->pLabel->Width, pSettings->pBinding->pLabel->Height);
		Gdiplus::GraphicsPath textPath;
		pSettings->pBinding->pLabel->AddToPath(&textPath, pHint->mHelpText, textRect);
		pGraphics->DrawPath(pSettings->pBinding->pMenu->pFont->pPen, &textPath);
		pGraphics->FillPath(pSettings->pBinding->pPrompt->pFont->pBrush, &textPath);
		offsetY += pSettings->pBinding->mLabelHeight;
	}
	return offsetY + 10;
}