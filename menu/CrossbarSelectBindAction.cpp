#include "CrossbarSelectBindAction.h"

CrossbarSelectBindAction::CrossbarSelectBindAction(FontMenuBase* pMainMenu, CrossbarActionType type, const char* buttonName)
	: FontMenuBase(pMainMenu, GetInitialState(pMainMenu->pAshitaCore, type, -1, buttonName))
	, mType(type)
	, mSkill(-1)
{
	strcpy_s(mButtonName, 256, buttonName);
}
CrossbarSelectBindAction::CrossbarSelectBindAction(FontMenuBase* pMainMenu, CrossbarActionType type, int skill, const char* buttonName)
	: FontMenuBase(pMainMenu, GetInitialState(pMainMenu->pAshitaCore, type, skill, buttonName))
	, mType(type)
	, mSkill(skill)
{
	strcpy_s(mButtonName, 256, buttonName);
}

void CrossbarSelectBindAction::HandleConfirm()
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
	if (strcmp(data.Result, "CANCEL") == 0)
	{
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}

	if (mType == CrossbarActionType::Ability)
	{
		SingleMacroInfo_t macro;
		macro.Type = IconType::Ability;

		int id = atoi(data.Result);
		IAbility* pResource = pAshitaCore->GetResourceManager()->GetAbilityById(id);
		macro.pResource = pResource;
		if (pResource->Targets == 1)
			sprintf_s(macro.IconCommand, 256, "/ja \"%s\" <me>", pResource->Name[0]);
		else
			sprintf_s(macro.IconCommand, 256, "/ja \"%s\" <t>", pResource->Name[0]);		
		strcpy_s(macro.IconText, 256, pResource->Name[0]);
		pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
		return;
	}

	if (mType == CrossbarActionType::Command)
	{
		SingleMacroInfo_t macro;
		macro.Type = IconType::Command;
		strcpy_s(macro.IconCommand, 256, data.Result);
		if (strcmp(data.Result, "/attack <t>") == 0)
			strcpy_s(macro.IconText, 256, "Attack");
		if (strcmp(data.Result, "/attackoff") == 0)
			strcpy_s(macro.IconText, 256, "Disengage");
		if (strcmp(data.Result, "/heal") == 0)
			strcpy_s(macro.IconText, 256, "Heal");
		if (strcmp(data.Result, "/logout") == 0)
			strcpy_s(macro.IconText, 256, "Logout");
		pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
		return;
	}

	if (mType == CrossbarActionType::Item)
	{
		SingleMacroInfo_t macro;
		macro.Type = IconType::Item;

		int id = atoi(data.Result);
		IItem* pResource = pAshitaCore->GetResourceManager()->GetItemById(id);
		macro.pResource = pResource;
		if (pResource->Targets == 1)
			sprintf_s(macro.IconCommand, 256, "/item \"%s\" <me>", pResource->Name[0]);
		else
			sprintf_s(macro.IconCommand, 256, "/item \"%s\" <t>", pResource->Name[0]);
		strcpy_s(macro.IconText, 256, pResource->Name[0]);
		pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
		return;
	}

	if (mType == CrossbarActionType::Spell)
	{
		SingleMacroInfo_t macro;
		macro.Type = IconType::Spell;

		int id = atoi(data.Result);
		ISpell* pResource = pAshitaCore->GetResourceManager()->GetSpellById(id);
		macro.pResource = pResource;
		if (pResource->Targets == 1)
			sprintf_s(macro.IconCommand, 256, "/ma \"%s\" <me>", pResource->Name[0]);
		else
			sprintf_s(macro.IconCommand, 256, "/ma \"%s\" <t>", pResource->Name[0]);
		strcpy_s(macro.IconText, 256, pResource->Name[0]);
		pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
		return;
	}

	if (mType == CrossbarActionType::Weaponskill)
	{
		SingleMacroInfo_t macro;
		macro.Type = IconType::Weaponskill;

		int id = atoi(data.Result);
		IAbility* pResource = pAshitaCore->GetResourceManager()->GetAbilityById(id);
		macro.pResource = pResource;
		if (pResource->Targets == 1)
			sprintf_s(macro.IconCommand, 256, "/ws \"%s\" <me>", pResource->Name[0]);
		else
			sprintf_s(macro.IconCommand, 256, "/ws \"%s\" <t>", pResource->Name[0]);
		strcpy_s(macro.IconText, 256, pResource->Name[0]);
		pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
		return;
	}
}

void CrossbarSelectBindAction::HandleButtonUp()
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

	mState = GetInitialState(pAshitaCore, mType, mSkill, mButtonName);
}

FontMenuState CrossbarSelectBindAction::GetInitialState(IAshitaCore* pAshitaCore, CrossbarActionType type, int skill, const char* buttonName)
{
	int mainJob = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJob();
	int mainJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJobLevel();
	int subJob = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJob();
	int subJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJobLevel();

	std::vector<FontMenuOption> options;
	if (type == CrossbarActionType::Command)
	{
		options.push_back(FontMenuOption("/attack <t>", "/attack <t>", false));
		options.push_back(FontMenuOption("/attackoff", "/attackoff", false));
		options.push_back(FontMenuOption("/heal", "/heal", false));
		options.push_back(FontMenuOption("/logout", "/logout", false));
		options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	}

	if (type == CrossbarActionType::Spell)
	{
		std::list<ISpell*> spells;
		for (int x = 0; x < 0x400; x++)
		{
			ISpell* pSpell = pAshitaCore->GetResourceManager()->GetSpellById(x);
			if ((pSpell == NULL) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasSpell(pSpell->Index)))
				continue;
			if ((skill != -1) && (pSpell->Skill != skill))
				continue;

			bool available = false;
			int mainRequired = pSpell->LevelRequired[mainJob];
			int subRequired = pSpell->LevelRequired[subJob];
			if ((mainRequired != -1) && (mainJobLevel >= mainRequired))
				available = true;
			if ((subRequired != -1) && (subJobLevel >= subRequired))
				available = true;
			//We aren't tracking job points here, so default to assuming we have them.
			if ((mainRequired > 99) && (mainJobLevel == 99))
				available = true;

			if (available)
			{
				spells.push_back(pSpell);
			}
		}
		spells.sort([](const ISpell* a, const ISpell* b)
			{
				return (strcmp(a->Name[0], b->Name[0]) < 0);
			});

		for (std::list<ISpell*>::iterator iter = spells.begin(); iter != spells.end(); iter++)
		{
			options.push_back(FontMenuOption((*iter)->Name[0], std::to_string((*iter)->Index).c_str(), false));
		}
		options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	}

	if (type == CrossbarActionType::Ability)
	{
		std::list<IAbility*> abilities;

		for (int x = 0x200; x < 0x600; x++)
		{
			IAbility* pAbility = pAshitaCore->GetResourceManager()->GetAbilityById(x);
			if ((!pAbility) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasAbility(pAbility->Id)))
				continue;

			abilities.push_back(pAbility);
		}

		abilities.sort([](const IAbility* a, const IAbility* b)
		{
				return (strcmp(a->Name[0], b->Name[0]) < 0);
		});

		for (std::list<IAbility*>::iterator iter = abilities.begin(); iter != abilities.end(); iter++)
		{
			options.push_back(FontMenuOption((*iter)->Name[0], std::to_string((*iter)->Id).c_str(), false));
		}
		options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	}

	if (type == CrossbarActionType::Weaponskill)
	{
		std::list<IAbility*> weaponskills;

		for (int x = 0; x < 0x200; x++)
		{
			IAbility* pAbility = pAshitaCore->GetResourceManager()->GetAbilityById(x);
			if ((!pAbility) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasAbility(pAbility->Id)))
				continue;

			weaponskills.push_back(pAbility);
		}

		weaponskills.sort([](const IAbility* a, const IAbility* b)
			{
				return (strcmp(a->Name[0], b->Name[0]) < 0);
			});

		for (std::list<IAbility*>::iterator iter = weaponskills.begin(); iter != weaponskills.end(); iter++)
		{
			options.push_back(FontMenuOption((*iter)->Name[0], std::to_string((*iter)->Id).c_str(), false));
		}
		options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	}

	if (type == CrossbarActionType::Item)
	{
		std::list<IItem*> items;

		int invMax = pAshitaCore->GetMemoryManager()->GetInventory()->GetContainerCountMax(0);
		for (int x = 1; x <= invMax; x++)
		{
			Ashita::FFXI::item_t* pItem = pAshitaCore->GetMemoryManager()->GetInventory()->GetContainerItem(0, x);
			if ((pItem->Id == 0) || (pItem->Count == 0))
				continue;
			IItem* pResource = pAshitaCore->GetResourceManager()->GetItemById(pItem->Id);
			if ((pResource == NULL) || ((pResource->Flags & 0x200) == NULL))
				continue;

			if (std::find(items.begin(), items.end(), pResource) == items.end())
			{
				items.push_back(pResource);
			}
		}

		items.sort([](const IItem* a, const IItem* b)
			{
				return (strcmp(a->Name[0], b->Name[0]) < 0);
			});

		for (std::list<IItem*>::iterator iter = items.begin(); iter != items.end(); iter++)
		{
			options.push_back(FontMenuOption((*iter)->Name[0], std::to_string((*iter)->Id).c_str(), false));
		}
		options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	}

	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonUp, "Refresh"),
		FontMenuHint(MacroButton::ButtonLeft, "Type In"),
		FontMenuHint(MacroButton::Cancel, "Return To Type Select")
	};

	return FontMenuState(options, hints, buttonName);
}

void CrossbarSelectBindAction::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (strcmp(data.Result, "BINDING_CONFIRMED") == 0)
	{
		mIsFinished = true;
		mCompletionData = data;
		return;
	}
}

void CrossbarSelectBindAction::HandleButtonLeft()
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
	if (mState.mAwaitingInput)
	{
		return;
	}

	mState.mAwaitingInput = true;
	if (mType == CrossbarActionType::Ability)
		wcscpy_s(mState.mInputPrompt, 2048, L"Enter an ability.");
	if (mType == CrossbarActionType::Command)
		wcscpy_s(mState.mInputPrompt, 2048, L"Enter a command.");
	if (mType == CrossbarActionType::Item)
		wcscpy_s(mState.mInputPrompt, 2048, L"Enter an item name.");
	if (mType == CrossbarActionType::Spell)
		wcscpy_s(mState.mInputPrompt, 2048, L"Enter a spell.");
	if (mType == CrossbarActionType::Weaponskill)
		wcscpy_s(mState.mInputPrompt, 2048, L"Enter a weaponskill.");
}
void CrossbarSelectBindAction::ReceiveText(const char* value)
{
    if (mIsHidden)
        return;

	if (pSubMenu)
	{
		pSubMenu->ReceiveText(value);
		return;
	}
	if (mState.mAwaitingInput)
	{
		mState.mAwaitingInput = false;

		if (mType == CrossbarActionType::Ability)
		{
			IAbility* pResource = pAshitaCore->GetResourceManager()->GetAbilityByName(value, 0);
			if ((pResource == NULL) || (pResource->Name[0] == NULL) || (strlen(pResource->Name[0]) < 3))
				return;

			SingleMacroInfo_t macro;
			macro.Type = IconType::Ability;
			macro.pResource = pResource;
			if (pResource->Targets == 1)
				sprintf_s(macro.IconCommand, 256, "/ja \"%s\" <me>", pResource->Name[0]);
			else
				sprintf_s(macro.IconCommand, 256, "/ja \"%s\" <t>", pResource->Name[0]);
			strcpy_s(macro.IconText, 256, pResource->Name[0]);
			pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
			return;
		}

		if (mType == CrossbarActionType::Command)
		{
			SingleMacroInfo_t macro;
			macro.Type = IconType::Command;
			strcpy_s(macro.IconCommand, 256, value);
			strcpy_s(macro.IconText, 256, value);
			pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
			return;
		}

		if (mType == CrossbarActionType::Item)
		{
			IItem* pResource = pAshitaCore->GetResourceManager()->GetItemByName(value, 0);
			if (!pResource)
				return;

			SingleMacroInfo_t macro;
			macro.Type = IconType::Item;
			macro.pResource = pResource;
			if (pResource->Targets == 1)
				sprintf_s(macro.IconCommand, 256, "/item \"%s\" <me>", pResource->Name[0]);
			else
				sprintf_s(macro.IconCommand, 256, "/item \"%s\" <t>", pResource->Name[0]);
			strcpy_s(macro.IconText, 256, pResource->Name[0]);
			pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
			return;
		}

		if (mType == CrossbarActionType::Spell)
		{
			ISpell* pResource = pAshitaCore->GetResourceManager()->GetSpellByName(value, 0);
			if ((pResource == NULL) || (pResource->Name[0] == NULL) || (strlen(pResource->Name[0]) < 3))
				return;

			SingleMacroInfo_t macro;
			macro.Type = IconType::Spell;
			macro.pResource = pResource;
			if (pResource->Targets == 1)
				sprintf_s(macro.IconCommand, 256, "/ma \"%s\" <me>", pResource->Name[0]);
			else
				sprintf_s(macro.IconCommand, 256, "/ma \"%s\" <t>", pResource->Name[0]);
			strcpy_s(macro.IconText, 256, pResource->Name[0]);
			pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
			return;
		}

		if (mType == CrossbarActionType::Weaponskill)
		{
			IAbility* pResource = pAshitaCore->GetResourceManager()->GetAbilityByName(value, 0);
			if ((pResource == NULL) || (pResource->Name[0] == NULL) || (strlen(pResource->Name[0]) < 3))
				return;

			SingleMacroInfo_t macro;
			macro.Type = IconType::Weaponskill;
			macro.pResource = pResource;
			if (pResource->Targets == 1)
				sprintf_s(macro.IconCommand, 256, "/ws \"%s\" <me>", pResource->Name[0]);
			else
				sprintf_s(macro.IconCommand, 256, "/ws \"%s\" <t>", pResource->Name[0]);
			strcpy_s(macro.IconText, 256, pResource->Name[0]);
			pSubMenu = new CrossbarSelectBindFlags(this, macro, mButtonName);
			return;
		}

	}
}