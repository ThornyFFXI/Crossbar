#include "ResonationTracker.h"

ResonationTracker::ResonationTracker(IAshitaCore* pAshitaCore)
    : pAshitaCore(pAshitaCore)
{}
ResonationTracker::~ResonationTracker()
{}

void ResonationTracker::Clear()
{
    mResonationMap.clear();
    mImmanenceMap.clear();
    mAffinityMap.clear();
}
SkillchainIndex ResonationTracker::GetResult(IAbility* pWeaponskill)
{
    std::map<int, ResonationInfo_t>::iterator iter = mResonationMap.find(GetTarget());
    if (iter == mResonationMap.end())
        return SkillchainIndex::None;

    SkillchainAttributes_t attributes = GetSkillchainAttributes(pWeaponskill->Id);
    if (attributes.Attribute[0] == SkillchainIndex::None)
        return SkillchainIndex::None;

    for (int x = 0; x < 3; x++)
    {
        SkillchainIndex currWs = attributes.Attribute[x];
        if (currWs == SkillchainIndex::None)
            return SkillchainIndex::None;

        for (int y = 0; y < 3; y++)
        {
            SkillchainIndex currAttr = iter->second.Attributes.Attribute[x];
            if (currAttr == SkillchainIndex::None)
                break;

            for (std::vector<SkillchainInfo_t>::iterator iter = mPossibleSkillchains.begin(); iter != mPossibleSkillchains.end(); iter++)
            {
                if ((iter->opener == currAttr) && (iter->closer == currWs))
                    return iter->result;
            }
        }
    }

    return SkillchainIndex::None;
}
WindowState ResonationTracker::GetWindow()
{
    std::map<int, ResonationInfo_t>::iterator iter = mResonationMap.find(GetTarget());
    if (iter == mResonationMap.end())
        return WindowState::Expired;

    if (std::chrono::steady_clock::now() < iter->second.WindowOpens)
    {
        return WindowState::NotOpen;
    }
    else if (std::chrono::steady_clock::now() > iter->second.WindowCloses)
    {
        return WindowState::Expired;
    }
    else
    {
        return WindowState::Open;
    }
}
int ResonationTracker::GetTimeUntilOpen()
{
    std::map<int, ResonationInfo_t>::iterator iter = mResonationMap.find(GetTarget());
    if (iter == mResonationMap.end())
        return 3600;

    if (std::chrono::steady_clock::now() < iter->second.WindowOpens)
    {
        auto compare = iter->second.WindowOpens - std::chrono::steady_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(compare).count();
        return max(0, value);
    }
    else if (std::chrono::steady_clock::now() > iter->second.WindowCloses)
    {
        return 3600;
    }
    else
    {
        return 0;
    }
}
void ResonationTracker::HandleActionPacket(uint32_t size, uint8_t* data)
{
    //Record resonation state..
    uint8_t actionType = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, 82, 4);
    if (actionType == 3) //WS Finish
    {
        uint32_t actionId = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, 86, 10);
        uint32_t actorId = Read32(data, 5);
        uint16_t buffId = 0;
        auto bitOffset = 150;
        while ((bitOffset + 40) < (size * 8))
        {
            uint32_t targetId = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 32);
            uint16_t skillchainMessage = 0;
            bitOffset += 36;

            bitOffset += 5;  //Reaction
            bitOffset += 12; //Animation
            bitOffset += 7;  //Effect
            bitOffset += 3;  //Knockback
            bitOffset += 17; //Param(buffid)
            bitOffset += 10; //MessageId
            bitOffset += 31; //???

            if (Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 1))
            {
                bitOffset += 1;  //Additional effect flag
                bitOffset += 10; //Additional effect damage
                bitOffset += 17; //Param
                skillchainMessage = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 10);
                bitOffset += 10; //Message
            }
            else
            {
                bitOffset += 1; //Additional effect flag
            }

            if (Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 1))
            {

                bitOffset += 1;  //Spikes effect flag
                bitOffset += 10; //Spikes damage
                bitOffset += 14; //Param
                bitOffset += 10; //Message
            }
            else
            {
                bitOffset += 1; //Spikes effect flag
            }
            std::map<int, SkillchainIndex>::iterator iMessage = mSkillchainMessageIds.find(skillchainMessage);
            std::map<int, ResonationInfo_t>::iterator iInfo = mResonationMap.find(targetId);

            if (iMessage != mSkillchainMessageIds.end())
            {
                if (iMessage->second == SkillchainIndex::None)
                {
                    if (iInfo != mResonationMap.end())
                    {
                        mResonationMap.erase(iInfo);
                    }
                }
                else if ((iInfo != mResonationMap.end()) && ((std::chrono::steady_clock::now() + std::chrono::seconds(1)) > iInfo->second.WindowOpens) && ((std::chrono::steady_clock::now() - std::chrono::seconds(1)) < iInfo->second.WindowCloses))
                {
                    bool finishSc = false;
                    if ((iMessage->second == SkillchainIndex::Light) || (iMessage->second == SkillchainIndex::Darkness))
                    {
                        SkillchainAttributes_t attr = GetSkillchainAttributes(actionId);
                        if ((iInfo->second.Attributes.Attribute[0] == iMessage->second) && (attr.Attribute[0] == iMessage->second))
                        {
                            mResonationMap.erase(iInfo);
                            finishSc = true;
                        }
                    }

                    if (!finishSc)
                    {
                        ResonationInfo_t info;
                        info.Depth = iInfo->second.Depth + 1;
                        info.Attributes = SkillchainAttributes_t(iMessage->second);
                        info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                        info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                        mResonationMap[targetId] = info;
                    }
                }
                else
                {
                    ResonationInfo_t info;
                    info.Depth = 1;
                    info.Attributes = SkillchainAttributes_t(iMessage->second);
                    info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                    info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                    mResonationMap[targetId] = info;
                }
            }
            else
            {
                ResonationInfo_t info;
                info.Depth = 0;
                info.Attributes = GetSkillchainAttributes(actionId);
                info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                mResonationMap[targetId] = info;
            }
        }
    }

    if (actionType == 4) //Spell finish
    {
        uint32_t actionId = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, 86, 10);
        uint32_t actorId = Read32(data, 5);
        uint16_t buffId = 0;
        auto bitOffset = 150;
        while ((bitOffset + 40) < (size * 8))
        {
            uint32_t targetId = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 32);
            uint16_t skillchainMessage = 0;
            bitOffset += 36;

            bitOffset += 5;  //Reaction
            bitOffset += 12; //Animation
            bitOffset += 7;  //Effect
            bitOffset += 3;  //Knockback
            bitOffset += 17; //Param(buffid)
            bitOffset += 10; //MessageId
            bitOffset += 31; //???

            if (Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 1))
            {
                bitOffset += 1;  //Additional effect flag
                bitOffset += 10; //Additional effect damage
                bitOffset += 17; //Param
                skillchainMessage = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 10);
                bitOffset += 10; //Message
            }
            else
            {
                bitOffset += 1; //Additional effect flag
            }

            if (Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, bitOffset, 1))
            {

                bitOffset += 1;  //Spikes effect flag
                bitOffset += 10; //Spikes damage
                bitOffset += 14; //Param
                bitOffset += 10; //Message
            }
            else
            {
                bitOffset += 1; //Spikes effect flag
            }
            std::map<int, SkillchainIndex>::iterator iMessage = mSkillchainMessageIds.find(skillchainMessage);
            if (iMessage != mSkillchainMessageIds.end())
            {
                std::map<int, ResonationInfo_t>::iterator iInfo = mResonationMap.find(targetId);
                if ((iInfo != mResonationMap.end()) && ((std::chrono::steady_clock::now() + std::chrono::seconds(1)) > iInfo->second.WindowOpens) && ((std::chrono::steady_clock::now() - std::chrono::seconds(1)) < iInfo->second.WindowCloses))
                {
                    ResonationInfo_t info;
                    info.Depth = iInfo->second.Depth + 1;
                    info.Attributes = SkillchainAttributes_t(iMessage->second);
                    info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                    info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                    mResonationMap[targetId] = info;
                }
                else
                {
                    ResonationInfo_t info;
                    info.Depth = 1;
                    info.Attributes = SkillchainAttributes_t(iMessage->second);
                    info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                    info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                    mResonationMap[targetId] = info;
                }
            }
            else
            {
                ISpell* pSpell = pAshitaCore->GetResourceManager()->GetSpellById(actionId);
                SkillchainAttributes_t skillchain;
                skillchain.Attribute[0] = SkillchainIndex::None;
                if (pSpell->Skill == 43)
                {
                    std::map<int, std::chrono::steady_clock::time_point>::iterator iter = mAffinityMap.find(actorId);
                    if ((iter != mAffinityMap.end()) && (std::chrono::steady_clock::now() < iter->second))
                    {
                        std::map<int, SkillchainAttributes_t>::iterator sIter = mSpellAttributes.find(actionId);
                        if (sIter != mSpellAttributes.end())
                        {
                            skillchain = sIter->second;
                            mAffinityMap.erase(iter);
                        }
                    }
                }
                else
                {
                    std::map<int, std::chrono::steady_clock::time_point>::iterator iter = mImmanenceMap.find(actorId);
                    if ((iter != mImmanenceMap.end()) && (std::chrono::steady_clock::now() < iter->second))
                    {
                        std::map<int, SkillchainAttributes_t>::iterator sIter = mSpellAttributes.find(actionId);
                        if (sIter != mSpellAttributes.end())
                        {
                            skillchain = sIter->second;
                            mImmanenceMap.erase(iter);
                        }
                    }
                }

                if (skillchain.Attribute[0] != SkillchainIndex::None)
                {
                    ResonationInfo_t info;
                    info.Depth = 0;
                    info.Attributes = skillchain;
                    info.WindowOpens = std::chrono::steady_clock::now() + std::chrono::milliseconds(3500);
                    info.WindowCloses = std::chrono::steady_clock::now() + std::chrono::milliseconds(9800 - (info.Depth * 1000));
                    mResonationMap[targetId] = info;
                }
            }
        }
    }

    if (actionType == 6)
    {
        if (actionType == 4) //Spell finish
        {
            uint32_t actionId = Ashita::BinaryData::UnpackBitsBE((uint8_t*)data, 0, 86, 10);
            uint32_t actorId = Read32(data, 5);

            if (actionId == 94)
            {
                mAffinityMap[actorId] = std::chrono::steady_clock::now() + std::chrono::seconds(29);
            }
            if (actionId == 317)
            {
                mImmanenceMap[actorId] = std::chrono::steady_clock::now() + std::chrono::seconds(59);
            }
        }
    }
}

SkillchainAttributes_t ResonationTracker::GetSkillchainAttributes(int index)
{
    SkillchainAttributes_t attr(SkillchainIndex::None, SkillchainIndex::None, SkillchainIndex::None);
    std::map<int, SkillchainAttributes_t>::iterator iter = mWeaponskillAttributes.find(index);
    if (iter != mWeaponskillAttributes.end())
    {
        attr = iter->second;
    }
    return attr;
}
int ResonationTracker::GetTarget()
{
    if (pAshitaCore->GetMemoryManager()->GetTarget()->GetIsSubTargetActive())
        return pAshitaCore->GetMemoryManager()->GetTarget()->GetServerId(1);
    else
        return pAshitaCore->GetMemoryManager()->GetTarget()->GetServerId(0);
}