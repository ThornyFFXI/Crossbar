#ifndef __ASHITA_CrossbarCanvas_H_INCLUDED__
#define __ASHITA_CrossbarCanvas_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#include "../config/CrossbarSettings.h"
#include "../config/CrossbarBindings.h"
#include "CrossbarMacroSet.h"
#include "GdiDIB.h"

class CrossbarCanvas
{
private:
	IAshitaCore* pAshitaCore;
	GdiDIB* pMainDisplay;
	IPrimitiveObject* pMainPrimitive;
	GdiDIB* pSubDisplay;
	IPrimitiveObject* pSubPrimitive;

	CrossbarBindings* pBindings;
	CrossbarSettings* pSettings;

	CrossbarMacroSet* pMacros[6];
    MacroMode mLastSingleMode;

public:
	CrossbarCanvas(IAshitaCore* pAshitaCore, CrossbarSettings* pSettings, CrossbarBindings* pBindings);
	~CrossbarCanvas();

	void Draw(MacroMode mode);
	void HandleButton(MacroButton button, MacroMode mode);
	void Hide();
	void UpdateBindings(CrossbarBindings* pNewBindings);
	void UpdatePalette();
};
#endif