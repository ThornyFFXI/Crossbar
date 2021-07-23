#ifndef __ASHITA_FontMenuDefines_H_INCLUDED__
#define __ASHITA_FontMenuDefines_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../Defines.h"

struct FontMenuCompletionData_t
{
    char Result[256];
    void* Data;
    int DataSize;
};
enum class FontMenuOptionType
{
    MenuOption, //Has no options, can be clicked to trigger event.
    CycleOption, //Allows user to cycle through options.
    InputOption //Allows user to type a value to change value.
};
class FontMenuOption
{
private:
    char mLabel[256];
    FontMenuOptionType mType;
    char mReturnValue[256];

    int iValue;
    std::vector<std::string> mValues;

public:
    FontMenuOption(const char* label, const char* returnValue, bool allowInput)
    {
        strcpy_s(mLabel, 256, label);
        strcpy_s(mReturnValue, 256, returnValue);
        if (allowInput)
            mType = FontMenuOptionType::InputOption;
        else
            mType = FontMenuOptionType::MenuOption;
    }
    FontMenuOption(const char* label, std::vector<std::string> values)
    {
        strcpy_s(mLabel, 256, label);
        mType = FontMenuOptionType::CycleOption;
        mValues = values;
        iValue = 0;
        strcpy_s(mReturnValue, 256, mValues[iValue].c_str());
    }
    virtual void Advance()
    {
        if (mType == FontMenuOptionType::CycleOption)
        {
            iValue++;
            if (iValue >= mValues.size())
                iValue = 0;
            strcpy_s(mReturnValue, 256, mValues[iValue].c_str());
        }
    }
    virtual const char* GetLabel()
    {
        return mLabel;
    }
    virtual FontMenuCompletionData_t GetReturn()
    {
        FontMenuCompletionData_t buffer;
        strcpy_s(buffer.Result, 256, mReturnValue);
        buffer.DataSize = 0;
        return buffer;
    }
    virtual FontMenuOptionType GetType()
    {
        return mType;
    }
    virtual const char* GetValue()
    {
        return mReturnValue;
    }
    virtual void SetReturn(const char* value)
    {
        strcpy_s(mReturnValue, 256, value);
    }
};
struct FontMenuHint
{
    MacroButton mButton;
    wchar_t mHelpText[256];

    FontMenuHint(MacroButton button, const char* text)
    {
        mButton = button;
        swprintf_s(mHelpText, 256, L"%S", text);
    }
};
class FontMenuState
{
public:
    wchar_t mHelpText[2048];

    bool mAwaitingInput;
    wchar_t mInputPrompt[2048];

    std::vector<FontMenuOption> mOptions;
    int mOptionMax;
    std::vector<FontMenuHint> mHints;
    int mHintMax;

    int mSelectedIndex;
    int mSelectedPosition;

    FontMenuState(std::vector<FontMenuOption> options, std::vector<FontMenuHint> hints, const char* helpText)
    {
        mOptions = options;
        mOptionMax = options.size();
        mHints = hints;
        mHintMax = hints.size();
        mAwaitingInput = false;
        mSelectedIndex = 0;
        mSelectedPosition = 0;
        swprintf_s(mHelpText, 2048, L"%S", helpText);
    }
    FontMenuState(std::vector<FontMenuOption> options, std::vector<FontMenuHint> hints, const char* helpText, int defaultIndex)
    {
        mOptions = options;
        mOptionMax = options.size();
        mHints = hints;
        mHintMax = hints.size();
        mAwaitingInput = false;
        mSelectedIndex = defaultIndex;
        if (defaultIndex < 9)
            mSelectedPosition = defaultIndex;
        else
            mSelectedPosition = 8;
        swprintf_s(mHelpText, 2048, L"%S", helpText);
    }
};

#endif