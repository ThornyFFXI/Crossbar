#include "CrossbarSelectBindType.h"

CrossbarSelectBindType::CrossbarSelectBindType(FontMenuBase* pMainMenu, SingleMacroInfo_t* pButton, const char* buttonName, MacroMode mode)
	: FontMenuBase(pMainMenu, GetInitialState(pMainMenu->pAshitaCore, buttonName, pMainMenu->pSettings->mConfig.UseLevelSync))
	, pButton(pButton)
	, mMode(mode)
{
	strcpy_s(mButtonName, 256, buttonName);
}

MacroMode CrossbarSelectBindType::GetModeOverride()
{
	return mMode;
}
void CrossbarSelectBindType::HandleConfirm()
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
	if (strcmp(data.Result, "SELECT_TYPE_COMMAND") == 0)
	{
		pSubMenu = new CrossbarSelectBindAction(this, CrossbarActionType::Command, mButtonName);
		return;
	}

	if (strncmp(data.Result, "SELECT_TYPE_MAGIC_", 18) == 0)
	{
		int skill = atoi(data.Result + 18);
		pSubMenu = new CrossbarSelectBindAction(this, CrossbarActionType::Spell, skill, mButtonName);
		return;
	}

	if (strcmp(data.Result, "SELECT_TYPE_ABILITY") == 0)
	{
		pSubMenu = new CrossbarSelectBindAction(this, CrossbarActionType::Ability, mButtonName);
		return;
	}

	if (strcmp(data.Result, "SELECT_TYPE_WEAPONSKILL") == 0)
	{
		pSubMenu = new CrossbarSelectBindAction(this, CrossbarActionType::Weaponskill, mButtonName);
		return;
	}

	if (strcmp(data.Result, "SELECT_TYPE_ITEM") == 0)
	{
		pSubMenu = new CrossbarSelectBindAction(this, CrossbarActionType::Item, mButtonName);
		return;
	}

	if (strcmp(data.Result, "SELECT_TYPE_CLEAR") == 0)
	{
		*pButton = SingleMacroInfo_t();
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "BINDING_CHANGED");
		return;
	}

	if (strcmp(data.Result, "CANCEL") == 0)
	{
		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "CANCEL");
		return;
	}
}
void CrossbarSelectBindType::HandleButtonUp()
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

	mState = GetInitialState(pAshitaCore, mButtonName, pSettings->mConfig.UseLevelSync);
}

FontMenuState CrossbarSelectBindType::GetInitialState(IAshitaCore* pAshitaCore, const char* button, bool useSync)
{
    int mainJob      = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJob();
    int mainJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetJobLevel(mainJob);
    int subJob       = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJob();
    int subJobLevel  = pAshitaCore->GetMemoryManager()->GetPlayer()->GetJobLevel(subJob);

    if (useSync)
    {
        mainJobLevel = pAshitaCore->GetMemoryManager()->GetPlayer()->GetMainJobLevel();
        subJobLevel  = pAshitaCore->GetMemoryManager()->GetPlayer()->GetSubJobLevel();
    }

	std::map<int, std::string> mSkillNames = {
        {0, "Trust Magic"},
		{32, "Divine Magic"},
		{33, "Healing Magic"},
		{34, "Enhancing Magic"},
		{35, "Enfeebling Magic"},
		{36, "Elemental Magic"},
		{37, "Dark Magic"},
		{38, "Summoning Magic"},
		{39, "Ninjutsu"},
		{40, "Songs"},
		{41, "Songs"},
		{42, "Songs"},
		{43, "Blue Magic" },
		{44, "Geomancy" },
		{45, "Geomancy" } };

	std::list<int> availableSkills;
	for (int x = 0; x < 0x400; x++)
	{
		ISpell* pSpell = pAshitaCore->GetResourceManager()->GetSpellById(x);
		if ((pSpell == NULL) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasSpell(pSpell->Index)))
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
			if (std::find(availableSkills.begin(), availableSkills.end(), pSpell->Skill) == availableSkills.end())
			{
				availableSkills.push_back(pSpell->Skill);
			}
		}
	}

	bool hasWeaponskill = false;
	for (int x = 0; x < 0x200; x++)
	{
		IAbility* pAbility = pAshitaCore->GetResourceManager()->GetAbilityById(x);
		if ((!pAbility) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasAbility(pAbility->Id)))
			continue;

		hasWeaponskill = true;
		break;
	}

	bool hasAbility = false;
	for (int x = 0x200; x < 0x600; x++)
	{
		IAbility* pAbility = pAshitaCore->GetResourceManager()->GetAbilityById(x);
		if ((!pAbility) || (!pAshitaCore->GetMemoryManager()->GetPlayer()->HasAbility(pAbility->Id)))
			continue;

		hasAbility = true;
		break;
	}


	std::vector<FontMenuOption> options;
	options.push_back(FontMenuOption("Command", "SELECT_TYPE_COMMAND", false));
	for (std::list<int>::iterator iter = availableSkills.begin(); iter != availableSkills.end(); iter++)
	{
		char nameBuffer[256];
		char resultBuffer[256];
		std::map<int, std::string>::iterator nameIter = mSkillNames.find(*iter);
		if (nameIter == mSkillNames.end())
			strcpy_s(nameBuffer, 256, "MAGIC_SKILL_ERROR");
		else
			strcpy_s(nameBuffer, 256, nameIter->second.c_str());
		sprintf_s(resultBuffer, 256, "SELECT_TYPE_MAGIC_%d", *iter);
		options.push_back(FontMenuOption(nameBuffer, resultBuffer, false));
	}
	if (hasAbility)
	{
		options.push_back(FontMenuOption("Job Ability", "SELECT_TYPE_ABILITY", false));
	}
	if (hasWeaponskill)
	{
		options.push_back(FontMenuOption("Weaponskill", "SELECT_TYPE_WEAPONSKILL", false));
	}
	options.push_back(FontMenuOption("Item", "SELECT_TYPE_ITEM", false));
	options.push_back(FontMenuOption("Clear", "SELECT_TYPE_CLEAR", false));
	options.push_back(FontMenuOption("Cancel", "CANCEL", false));
	
	std::vector<FontMenuHint> hints = {
		FontMenuHint(MacroButton::Confirm, "Select Option"),
		FontMenuHint(MacroButton::ButtonUp, "Refresh"),
		FontMenuHint(MacroButton::Cancel, "Return To Button Select")
	};

	return FontMenuState(options, hints, button);
}

void CrossbarSelectBindType::HandleSubMenu(FontMenuCompletionData_t data)
{
	if (strcmp(data.Result, "BINDING_CONFIRMED") == 0)
	{
		*pButton = *reinterpret_cast<SingleMacroInfo_t*>(data.Data);
		mIsFinished = true;
		free(data.Data);

		mIsFinished = true;
		strcpy_s(mCompletionData.Result, 256, "BINDING_CHANGED");
		return;
	}
}