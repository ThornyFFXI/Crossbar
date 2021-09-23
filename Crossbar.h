#ifndef __ASHITA_Crossbar_H_INCLUDED__
#define __ASHITA_Crossbar_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "Defines.h"
#include "input/InputHandler.h"
#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "input/CrossbarXInput.h"
#include "input/CrossbarDirectInput.h"
#include "display/CrossbarCanvas.h"
#include "menu/CrossbarMainMenu.h"

class Crossbar : IPlugin
{
private:
    IAshitaCore* m_AshitaCore;
    ILogManager* m_LogManager;
    IDirect3DDevice8* m_Direct3DDevice;
    uint32_t m_PluginId;
    InputHandler* pInput;
    CrossbarSettings* pSettings;
    CrossbarBindings* pBindings;
    CrossbarXInput* pXInput;
    CrossbarDirectInput* pDirectInput;
    CrossbarCanvas* pCanvas;
    FontMenuBase* pMenu;
    ULONG_PTR mGDIToken;

    uint32_t mCurrentId;
    uint8_t mCurrentJob;
    char mCurrentName[256];
    MacroMode mCurrentMode;
    bool mZoning;
    DWORD pGameMenu;
    DWORD pMenuHelp;

public:
    const char* GetName(void) const override
    {
        return "Crossbar";
    }
    const char* GetAuthor(void) const override
    {
        return "Thorny";
    }
    const char* GetDescription(void) const override
    {
        return "Insert description here.";
    }
    const char* GetLink(void) const override
    {
        return "Insert link here.";
    }
    double GetVersion(void) const override
    {
        return 1.14f;
    }
    int32_t GetPriority(void) const override
    {
        return 0;
    }
    uint32_t GetFlags(void) const override
    {
        return (uint32_t)Ashita::PluginFlags::All;
    }

    bool Initialize(IAshitaCore* core, ILogManager* logger, const uint32_t id) override;
    void Release(void) override;

    bool GetMenuActive();
    bool GetGameMenuActive();
    std::string GetMenuName();
    std::string GetMenuHelpText();
    void HandleButtonPress(MacroButton button);
    void HandleMenuCombo();
    void SetMacroMode(MacroMode mode);

    // Event Callbacks: ChatManager
    bool HandleCommand(int32_t mode, const char* command, bool injected) override;

    // Event Callbacks: PacketManager
    bool HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;
    bool HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;

    // Event Callbacks: Direct3D
    bool Direct3DInitialize(IDirect3DDevice8* device) override;
    void Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) override;

    void InitializeCrossbar();
};

#endif