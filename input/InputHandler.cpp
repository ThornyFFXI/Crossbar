#include "InputHandler.h"

InputHandler::InputHandler(Crossbar* pCrossbar)
	: pCrossbar(pCrossbar)
	, mConfig(InputConfig_t())
	, mLastState(InputData_t())
	, mRightShoulderFirst(false)
	, mLeftTap(false)
	, mLeftTapTimer(std::chrono::steady_clock::now())
	, mRightTap(false)
	, mRightTapTimer(std::chrono::steady_clock::now())
	, mMenuCombo(MenuComboState::Inactive)
	, mMenuTimer(std::chrono::steady_clock::now())
{
	
}

bool InputHandler::GetMenuActive()
{
	return pCrossbar->GetMenuActive();
}
void InputHandler::HandleState(InputData_t input)
{
	pCrossbar->SetMacroMode(GetMacroMode(input));
	HandleMenuCombo(input);
	HandleButtons(input);
	mLastState = input;
}

MacroMode InputHandler::GetMacroMode(InputData_t input)
{
	if (input.LeftTrigger)
	{
		//We know this is a fresh hit, not just held.
		if (!mLastState.LeftTrigger)
		{
			if (std::chrono::steady_clock::now() < mLeftTapTimer)
			{
				mLeftTap = true;
			}
			mLeftTapTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(mConfig.TapDuration);
			if (!mLastState.RightTrigger)
			{
				mRightShoulderFirst = false;
			}
		}
	}
	else
	{
		mLeftTap = false;
	}

	if (input.RightTrigger)
	{
		//We know this is a fresh hit, not just held.
		if (!mLastState.RightTrigger)
		{
			if (std::chrono::steady_clock::now() < mRightTapTimer)
			{
				mRightTap = true;
			}
			mRightTapTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(mConfig.TapDuration);
			if (!input.LeftTrigger)
			{
				mRightShoulderFirst = true;
			}
		}
	}
	else
	{
		mRightTap = false;
	}


	if (input.LeftTrigger)
	{
		if (input.RightTrigger)
		{
			if ((mConfig.AllowPriority) && (mRightShoulderFirst))
				return MacroMode::BothTriggersRT;
			else
				return MacroMode::BothTriggersLT;
		}
		else if ((mConfig.AllowDoubleTap) && (mLeftTap))
			return MacroMode::LeftTriggerDT;
		else
			return MacroMode::LeftTrigger;
	}
	else if (input.RightTrigger)
	{
		if ((mConfig.AllowDoubleTap) && (mRightTap))
			return MacroMode::RightTriggerDT;
		else
			return MacroMode::RightTrigger;
	}
	else
	{
		return MacroMode::NoTrigger;
	}
}

void InputHandler::HandleMenuCombo(InputData_t input)
{
	if (input.LeftShoulder && input.LeftTrigger && input.RightShoulder && input.RightTrigger)
	{
		if (mMenuCombo == MenuComboState::Inactive)
		{
			mMenuCombo = MenuComboState::Waiting;
			mMenuTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(mConfig.MenuDuration);
		}
		else if ((mMenuCombo == MenuComboState::Waiting) && (std::chrono::steady_clock::now() > mMenuTimer))
		{
			pCrossbar->HandleMenuCombo();
			mMenuCombo = MenuComboState::Triggered;
		}
	}
	else
	{
		mMenuCombo = MenuComboState::Inactive;
	}
}
void InputHandler::HandleButtons(InputData_t input)
{
	if ((input.LeftShoulder) && (!mLastState.LeftShoulder))
		pCrossbar->HandleButtonPress(MacroButton::ShoulderLeft);
	if ((input.RightShoulder) && (!mLastState.RightShoulder))
		pCrossbar->HandleButtonPress(MacroButton::ShoulderRight);
	for (int x = 0; x < 4; x++)
	{
		if ((input.Dpad[x]) && (!mLastState.Dpad[x]))
			pCrossbar->HandleButtonPress((MacroButton)x);
		if ((input.Buttons[x]) && (!mLastState.Buttons[x]))
			pCrossbar->HandleButtonPress((MacroButton)(x + 4));
	}
}
void InputHandler::LoadConfig(InputConfig_t config)
{
	mConfig = config;
}