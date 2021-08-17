#include "Crossbar.h"
#pragma comment(lib, "psapi.lib")
#include <psapi.h>

ResonationTracker* CrossbarWeaponskillMacro::pResonation = NULL;
DWORD CrossbarItemMacro::pRealTime                   = NULL;

__declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);

    return (IPlugin*)(new Crossbar());
}

__declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}

bool Crossbar::Initialize(IAshitaCore* core, ILogManager* logger, const uint32_t id)
{
    m_AshitaCore = core;
    m_LogManager = logger;
    m_PluginId = id;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&mGDIToken, &gdiplusStartupInput, NULL);
	pInput = new InputHandler(this);
	pDirectInput = new CrossbarDirectInput(pInput);
	pXInput = new CrossbarXInput(pInput);	
	CrossbarWeaponskillMacro::pResonation = new ResonationTracker(m_AshitaCore);
	pMenu = NULL;
	pCanvas = NULL;
	pBindings = NULL;
    pSettings                             = NULL;
    mZoning                               = false;
	
    MODULEINFO mod = {0};
    ::GetModuleInformation(::GetCurrentProcess(), ::GetModuleHandle("FFXiMain.dll"), &mod, sizeof(MODULEINFO));
    CrossbarItemMacro::pRealTime = Ashita::Memory::FindPattern((uintptr_t)mod.lpBaseOfDll, (uintptr_t)mod.SizeOfImage, "8B0D????????8B410C8B49108D04808D04808D04808D04C1C3", 2, 0);
	pGameMenu = Ashita::Memory::FindPattern((uintptr_t)mod.lpBaseOfDll, (uintptr_t)mod.SizeOfImage, "8B480C85C974??8B510885D274??3B05", 16, 0);

	if (m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberIsActive(0))
	{
		strcpy_s(mCurrentName, 256, m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(0));
		mCurrentId = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberServerId(0);
		mCurrentJob = m_AshitaCore->GetMemoryManager()->GetPlayer()->GetMainJob();
		InitializeCrossbar();
	}

    return true;
}

void Crossbar::Release(void)
{	
	SAFE_DELETE(CrossbarWeaponskillMacro::pResonation);
	SAFE_DELETE(pMenu);
	SAFE_DELETE(pCanvas);
	SAFE_DELETE(pBindings);
	SAFE_DELETE(pSettings);
	SAFE_DELETE(pXInput);
	SAFE_DELETE(pDirectInput);
	delete pInput;
	Gdiplus::GdiplusShutdown(mGDIToken);
}

bool Crossbar::HandleCommand(int32_t mode, const char* command, bool injected)
{
	if (_strnicmp(command, "/cbm ", 5) == 0)
	{
		if (pMenu)
		{
			pMenu->ReceiveText(command + 5);
		}
		return true;
	}
	return false;
}

bool Crossbar::HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(size);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(modified);
	UNREFERENCED_PARAMETER(sizeChunk);
	UNREFERENCED_PARAMETER(dataChunk);
	UNREFERENCED_PARAMETER(injected);
	UNREFERENCED_PARAMETER(blocked);

	if (id == 0x00A)
	{
		char newName[256] = { 0 };
		memcpy(newName, data + 0x84, 16);
		uint32_t newId = Read32(data, 4);
		CrossbarWeaponskillMacro::pResonation->Clear();
		if ((mCurrentId != newId) || (strcmp(mCurrentName, newName) != 0))
		{
			memcpy(mCurrentName, newName, 256);
			mCurrentId = newId;
			mCurrentJob = Read8(data, 0xB4);
			InitializeCrossbar();
		}
        mZoning = false;
	}
	if (id == 0x00B)
	{
        mZoning = true;
	}
	if (id == 0x28)
	{
		CrossbarWeaponskillMacro::pResonation->HandleActionPacket(size, (uint8_t*)data);
	}
	if (id == 0x61)
	{
		int newJob = Read8(data, 0x0C);
		if ((newJob != 0) && (newJob != mCurrentJob))
		{
			mCurrentJob = newJob;
			InitializeCrossbar();
		}
	}

	return false;
}

bool Crossbar::HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
	UNREFERENCED_PARAMETER(id);
	UNREFERENCED_PARAMETER(size);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(modified);
	UNREFERENCED_PARAMETER(sizeChunk);
	UNREFERENCED_PARAMETER(dataChunk);
	UNREFERENCED_PARAMETER(injected);
	UNREFERENCED_PARAMETER(blocked);

	return false;
}

bool Crossbar::Direct3DInitialize(IDirect3DDevice8* device)
{
	this->m_Direct3DDevice = device;

	return true;
}

void Crossbar::Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	UNREFERENCED_PARAMETER(pSourceRect);
	UNREFERENCED_PARAMETER(pDestRect);
	UNREFERENCED_PARAMETER(hDestWindowOverride);
	UNREFERENCED_PARAMETER(pDirtyRegion);

	if (pDirectInput)
	{
		if (pDirectInput->AttemptHook())
		{
			m_AshitaCore->GetChatManager()->Write(0, false, "Successfully hooked DirectInput.");
		}
	}
	if (pXInput)
	{
		if (pXInput->AttemptHook())
		{
			m_AshitaCore->GetChatManager()->Write(0, false, "Successfully hooked XInput.");
		}
	}

	
	bool draw   = true;
    int myIndex = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);
    if ((mZoning)
		|| (myIndex < 1)
		|| (m_AshitaCore->GetMemoryManager()->GetEntity()->GetRawEntity(myIndex) == 0)
		|| ((m_AshitaCore->GetMemoryManager()->GetEntity()->GetRenderFlags0(myIndex) & 0x200) == 0)
		|| ((m_AshitaCore->GetMemoryManager()->GetEntity()->GetRenderFlags0(myIndex) & 0x4000) != 0)
		|| (m_AshitaCore->GetMemoryManager()->GetEntity()->GetStatus(myIndex) == 4))
    {
        if (pCanvas)
        {
            pCanvas->Hide();
        }
        
		if (pMenu)
        {
            pMenu->Hide();
        }
        return;
	}

	if (pMenu)
	{
		FontMenuCompletionData_t data;
		if (pMenu->GetIsFinished(&data))
		{
			delete pMenu;
			pMenu = 0;
		}
		else
		{
			pMenu->Draw();
		}
	}
	if (pCanvas)
	{
		MacroMode drawMode = mCurrentMode;
		if (pMenu)
		{
			MacroMode overrideMode = pMenu->GetModeOverride();
			if (overrideMode != MacroMode::NoTrigger)
			{
				drawMode = overrideMode;
			}
		}
		pCanvas->Draw(drawMode);
	}
}
bool Crossbar::GetMenuActive()
{
	return (pMenu != NULL);
}
bool Crossbar::GetGameMenuActive()
{
    if (pGameMenu == NULL)
        return false;

    DWORD pSubMenu = Read32(pGameMenu, 0);
    pSubMenu       = Read32(pSubMenu, 0);
    if (pSubMenu == NULL)
        return false;

    DWORD pMenuHeader = Read32(pSubMenu, 4);
    char buffer[17]   = {0};
    memcpy(buffer, (const char*)(pMenuHeader + 0x46), 16);
    return ((strstr(buffer, "playermo") == 0) && (strstr(buffer, "inline") == 0));
}
void Crossbar::HandleButtonPress(MacroButton button)
{
	if (button == MacroButton::ShoulderLeft)
	{
		if ((mCurrentMode == MacroMode::LeftTrigger) || (mCurrentMode == MacroMode::LeftTriggerDT))
		{
			if (pBindings)
			{
				if (pBindings->pJobSettings)
				{
					if (pBindings->pJobSettings->mPaletteList.size() > 1)
					{
						if (pBindings->pJobSettings->mPaletteIterator == pBindings->pJobSettings->mPaletteList.begin())
						{
							pBindings->pJobSettings->mPaletteIterator = pBindings->pJobSettings->mPaletteList.end();
						}
						pBindings->pJobSettings->mPaletteIterator--;
						pBindings->mRedraw = true;
					}
				}
			}
		}
		return;
	}

	if (button == MacroButton::ShoulderRight)
	{
		if ((mCurrentMode == MacroMode::RightTrigger) || (mCurrentMode == MacroMode::RightTriggerDT))
		{
			if (pBindings)
			{
				if (pBindings->pJobSettings)
				{
					if (pBindings->pJobSettings->mPaletteList.size() > 1)
					{
						pBindings->pJobSettings->mPaletteIterator++;
						if (pBindings->pJobSettings->mPaletteIterator == pBindings->pJobSettings->mPaletteList.end())
						{
							pBindings->pJobSettings->mPaletteIterator = pBindings->pJobSettings->mPaletteList.begin();
						}
						pBindings->mRedraw = true;
					}
				}
			}
		}
		return;
	}

	if (pMenu)
	{
		if (mCurrentMode == MacroMode::NoTrigger)
		{
			if (button == MacroButton::DpadUp)
				pMenu->HandleDpadUp();
			if (button == MacroButton::DpadRight)
				pMenu->HandleDpadRight();
			if (button == MacroButton::DpadDown)
				pMenu->HandleDpadDown();
			if (button == MacroButton::DpadLeft)
				pMenu->HandleDpadLeft();
			if (button == MacroButton::ButtonUp)
				pMenu->HandleButtonUp();
			if (button == MacroButton::ButtonRight)
				pMenu->HandleButtonRight();
			if (button == MacroButton::ButtonDown)
				pMenu->HandleButtonDown();
			if (button == MacroButton::ButtonLeft)
				pMenu->HandleButtonLeft();
		}
		else
		{
			pMenu->HandleMacro(button, mCurrentMode);
		}			
	}
	else if ((pCanvas) && (mCurrentMode != MacroMode::NoTrigger) && ((int)button < 8))
	{
		pCanvas->HandleButton(button, mCurrentMode);
	}
}
void Crossbar::HandleMenuCombo()
{
	if (pMenu)
	{
		delete pMenu;
		pMenu = NULL;
	}
	else
	{
		pMenu = new CrossbarMainMenu(m_AshitaCore, pSettings, pBindings);
	}
}

void Crossbar::SetMacroMode(MacroMode mode)
{
	mCurrentMode = mode;
}

void Crossbar::InitializeCrossbar()
{
	SAFE_DELETE(pMenu);
	pMenu = NULL;
	SAFE_DELETE(pCanvas);
	pCanvas = NULL;
	SAFE_DELETE(pBindings);
	pBindings = NULL;
	SAFE_DELETE(pSettings);
	pSettings = NULL;

	pSettings = new CrossbarSettings(m_AshitaCore, mCurrentName, mCurrentId);
	if (!pSettings->mIsLoaded)
	{
		m_AshitaCore->GetChatManager()->Write(0, false, "Crossbar: Failed to load theme.");
		delete pSettings;
		pSettings = NULL;
		return;
	}
	pInput->LoadConfig(pSettings->mInput);
	pBindings = new CrossbarBindings(m_AshitaCore, mCurrentName, mCurrentId, mCurrentJob, pSettings);
	pCanvas = new CrossbarCanvas(m_AshitaCore, pSettings, pBindings);
}