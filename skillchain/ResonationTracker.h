#ifndef __ASHITA_ResonationTracker_H_INCLUDED__
#define __ASHITA_ResonationTracker_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "../Defines.h"
#include "../config/CrossbarSettings.h"
#include <map>

enum class WindowState
{
    NotOpen = 0,
    Open = 1,
    Expired = 2
};

struct SkillchainAttributes_t
{
    SkillchainIndex Attribute[3];

    SkillchainAttributes_t()
    {
        Attribute[0] = SkillchainIndex::None;
        Attribute[1] = SkillchainIndex::None;
        Attribute[2] = SkillchainIndex::None;
    }
    SkillchainAttributes_t(SkillchainIndex index)
    {
        Attribute[0] = index;
        Attribute[1] = SkillchainIndex::None;
        Attribute[2] = SkillchainIndex::None;
    };
    SkillchainAttributes_t(SkillchainIndex primary, SkillchainIndex secondary, SkillchainIndex tertiary)
    {
        Attribute[0] = primary;
        Attribute[1] = secondary;
        Attribute[2] = tertiary;
    };
};

struct SkillchainInfo_t
{
    SkillchainIndex result;
    SkillchainIndex opener;
    SkillchainIndex closer;

    SkillchainInfo_t();
    SkillchainInfo_t(SkillchainIndex result, SkillchainIndex opener, SkillchainIndex closer)
        : opener(opener)
        , closer(closer)
        , result(result)
    {}
};

struct ResonationInfo_t
{
    int Depth;
    SkillchainAttributes_t Attributes;
    std::chrono::steady_clock::time_point WindowOpens;
    std::chrono::steady_clock::time_point WindowCloses;
};

class ResonationTracker
{
    IAshitaCore* pAshitaCore;
    std::map<int, std::chrono::steady_clock::time_point> mAffinityMap;
    std::map<int, std::chrono::steady_clock::time_point> mImmanenceMap;
    std::map<int, ResonationInfo_t> mResonationMap;

    std::map<int, SkillchainIndex> mSkillchainMessageIds = {
        { 288, SkillchainIndex::Light },
        { 289, SkillchainIndex::Darkness },
        { 290, SkillchainIndex::Gravitation },
        { 291, SkillchainIndex::Fragmentation },
        { 292, SkillchainIndex::Distortion },
        { 293, SkillchainIndex::Fusion },
        { 294, SkillchainIndex::Compression },
        { 295, SkillchainIndex::Liquefaction },
        { 296, SkillchainIndex::Induration },
        { 297, SkillchainIndex::Reverberation },
        { 298, SkillchainIndex::Transfixion },
        { 299, SkillchainIndex::Scission },
        { 300, SkillchainIndex::Detonation },
        { 301, SkillchainIndex::Impaction },
        { 385, SkillchainIndex::Light },
        { 386, SkillchainIndex::Darkness },
        { 387, SkillchainIndex::Gravitation },
        { 388, SkillchainIndex::Fragmentation },
        { 389, SkillchainIndex::Distortion },
        { 390, SkillchainIndex::Fusion },
        { 391, SkillchainIndex::Compression },
        { 392, SkillchainIndex::Liquefaction },
        { 393, SkillchainIndex::Induration },
        { 394, SkillchainIndex::Reverberation },
        { 395, SkillchainIndex::Transfixion },
        { 396, SkillchainIndex::Scission },
        { 397, SkillchainIndex::Detonation },
        { 398, SkillchainIndex::Impaction },
        { 767, SkillchainIndex::None },
        { 768, SkillchainIndex::None },
        { 769, SkillchainIndex::None },
        { 770, SkillchainIndex::None }
    };
    std::map<int, SkillchainAttributes_t> mSpellAttributes = {
        { 144, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Fire
        { 145, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Fire II
        { 146, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Fire III
        { 147, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Fire IV
        { 148, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Fire V
        { 149, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Blizzard
        { 150, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Blizzard II
        { 151, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Blizzard III
        { 152, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Blizzard IV
        { 153, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Blizzard V
        { 154, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Aero
        { 155, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Aero II
        { 156, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Aero III
        { 157, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Aero IV
        { 158, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Aero V
        { 159, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Stone
        { 160, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Stone II
        { 161, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Stone III
        { 162, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Stone IV
        { 163, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Stone V
        { 164, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Thunder
        { 165, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Thunder II
        { 166, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Thunder III
        { 167, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Thunder IV
        { 168, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Thunder V
        { 169, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Water
        { 170, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Water II
        { 171, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Water III
        { 172, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Water IV
        { 173, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Water V
        { 278, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Geohelix
        { 279, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Hydrohelix
        { 280, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Anemohelix
        { 281, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Pyrohelix
        { 282, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Cryohelix
        { 283, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Ionohelix
        { 284, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Noctohelix
        { 285, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Luminohelix
        { 503, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Impact
        { 519, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Screwdriver
        { 527, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Smite of Rage
        { 529, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Bludgeon
        { 539, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Terror Touch
        { 540, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Spinal Cleave
        { 543, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Mandibular Bite
        { 545, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Sickle Slash
        { 551, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Power Attack
        { 554, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Death Scissors
        { 560, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Frenetic Rip
        { 564, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Body Slam
        { 567, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Helldive
        { 569, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Jet Stream
        { 577, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Foot Kick
        { 585, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Ram Charge
        { 587, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Claw Cyclone
        { 589, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Dimensional Death
        { 594, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Uppercut
        { 596, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Pinecone Bomb
        { 597, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Sprout Smack
        { 599, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Queasyshroom
        { 603, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Wild Oats
        { 611, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::None, SkillchainIndex::None) }, //Disseverment
        { 617, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::None, SkillchainIndex::None) }, //Vertical Cleave
        { 620, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Battle Dance
        { 622, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Grand Slam
        { 623, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Head Butt
        { 628, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Frypan
        { 631, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Hydro Shot
        { 638, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Feather Storm
        { 640, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Tail Slap
        { 641, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Hysteric Barrage
        { 643, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Cannonball
        { 650, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Seedspray
        { 652, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Spiral Spin
        { 653, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Asuran Claws
        { 654, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Sub\-zero Smash
        { 665, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Final Sting
        { 666, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Goblin Rush
        { 667, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Vanity Dive
        { 669, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Whirl of Rage
        { 670, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Benthic Typhoon
        { 673, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Quad. Continuum
        { 677, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Scission, SkillchainIndex::None) }, //Empty Thrash
        { 682, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Delta Thrust
        { 688, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Heavy Strike
        { 692, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Sudden Lunge
        { 693, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Scission, SkillchainIndex::Impaction) }, //Quadrastrike
        { 697, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::None, SkillchainIndex::None) }, //Amorphic Spikes
        { 699, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Barbed Crescent
        { 704, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::None, SkillchainIndex::None) }, //Paralyzing Triad
        { 706, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Glutinous Dart
        { 709, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Thrashing Assault
        { 714, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Sinker Drill
        { 723, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Saurian Slide
        { 740, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Tourbillion
        { 742, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Bilgestorm
        { 743, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Bloodrake
        { 885, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Geohelix II
        { 886, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Hydrohelix II
        { 887, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Anemohelix II
        { 888, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Pyrohelix II
        { 889, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Cryohelix II
        { 890, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Ionohelix II
        { 891, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Noctohelix II
        { 892, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) } //Luminohelix II
    };
    std::map<int, SkillchainAttributes_t> mWeaponskillAttributes = {
        { 1, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Combo
        { 2, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Shoulder Tackle
        { 3, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //One Inch Punch
        { 4, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Backhand Blow
        { 5, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Raging Fists
        { 6, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Spinning Attack
        { 7, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Howling Fist
        { 8, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Dragon Kick
        { 9, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Liquefaction, SkillchainIndex::None) }, //Asuran Fists
        { 10, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) }, //Final Heaven
        { 11, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Ascetic's Fury
        { 12, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Liquefaction, SkillchainIndex::None) }, //Stringing Pummel
        { 13, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::Impaction) }, //Tornado Kick
        { 14, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Victory Smite
        { 15, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Shijin Spiral
        { 16, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Wasp Sting
        { 17, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Viper Bite
        { 18, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Shadowstitch
        { 19, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Gust Slash
        { 20, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Cyclone
        { 23, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Dancing Edge
        { 24, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Shark Bite
        { 25, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Evisceration
        { 26, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Mercy Stroke
        { 27, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Compression, SkillchainIndex::None) }, //Mandalic Stab
        { 28, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Mordant Rime
        { 29, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Pyrrhic Kleos
        { 30, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::Impaction) }, //Aeolian Edge
        { 31, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Rudra's Storm
        { 32, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Fast Blade
        { 33, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Burning Blade
        { 34, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Red Lotus Blade
        { 35, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Flat Blade
        { 36, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Shining Blade
        { 37, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Seraph Blade
        { 38, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Circle Blade
        { 40, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Vorpal Blade
        { 41, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::None, SkillchainIndex::None) }, //Swift Blade
        { 42, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Savage Blade
        { 43, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) }, //Knights of Round
        { 44, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Death Blossom
        { 45, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Atonement
        { 46, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Expiacion
        { 48, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Hard Slash
        { 49, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Power Slash
        { 50, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Frostbite
        { 51, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Freezebite
        { 52, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Shockwave
        { 53, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Crescent Moon
        { 54, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Sickle Moon
        { 55, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::None, SkillchainIndex::None) }, //Spinning Slash
        { 56, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Ground Strike
        { 57, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) }, //Scourge
        { 58, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::Impaction) }, //Herculean Slash
        { 59, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Torcleaver
        { 60, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Resolution
        { 61, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Dimidiation
        { 64, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Raging Axe
        { 65, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Smash Axe
        { 66, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Gale Axe
        { 67, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Avalanche Axe
        { 68, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Scission, SkillchainIndex::Impaction) }, //Spinning Axe
        { 69, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Rampage
        { 70, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Calamity
        { 71, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Mistral Axe
        { 72, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Decimation
        { 73, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Onslaught
        { 74, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Primal Rend
        { 75, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Bora Axe
        { 76, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Cloudsplitter
        { 77, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Ruinator
        { 80, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Shield Break
        { 81, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Iron Tempest
        { 82, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Sturmwind
        { 83, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Armor Break
        { 84, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Keen Edge
        { 85, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Weapon Break
        { 86, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Raging Rush
        { 87, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::None, SkillchainIndex::None) }, //Full Break
        { 88, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Steel Cyclone
        { 89, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) }, //Metatron Torment
        { 90, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Scission, SkillchainIndex::None) }, //King's Justice
        { 91, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::Impaction) }, //Fell Cleave
        { 92, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Ukko's Fury
        { 93, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Compression, SkillchainIndex::None) }, //Upheaval
        { 96, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Slice
        { 97, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Dark Harvest
        { 98, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Shadow of Death
        { 99, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Scission, SkillchainIndex::None) }, //Nightmare Scythe
        { 100, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Spinning Scythe
        { 101, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Vorpal Scythe
        { 102, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Guillotine
        { 103, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::None, SkillchainIndex::None) }, //Cross Reaper
        { 104, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Spiral Hell
        { 105, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Catastrophe
        { 106, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Compression, SkillchainIndex::None) }, //Insurgency
        { 107, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Infernal Scythe
        { 108, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Quietus
        { 109, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Entropy
        { 112, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Double Thrust
        { 113, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Thunder Thrust
        { 114, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Raiden Thrust
        { 115, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Leg Sweep
        { 116, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Penta Thrust
        { 117, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Vorpal Thrust
        { 118, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Skewer
        { 119, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Wheeling Thrust
        { 120, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Induration, SkillchainIndex::None) }, //Impulse Drive
        { 121, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Geirskogul
        { 122, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Drakesbane
        { 123, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Sonic Thrust
        { 124, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Camlann's Torment
        { 125, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Stardiver
        { 128, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::None, SkillchainIndex::None) }, //Blade: Rin
        { 129, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::None, SkillchainIndex::None) }, //Blade: Retsu
        { 130, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Blade: Teki
        { 131, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Blade: To
        { 132, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Blade: Chi
        { 133, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::None, SkillchainIndex::None) }, //Blade: Ei
        { 134, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Blade: Jin
        { 135, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::None, SkillchainIndex::None) }, //Blade: Ten
        { 136, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Blade: Ku
        { 137, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Blade: Metsu
        { 138, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Compression, SkillchainIndex::None) }, //Blade: Kamu
        { 139, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Blade: Yu
        { 140, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Blade: Hi
        { 141, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Blade: Shun
        { 144, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Scission, SkillchainIndex::None) }, //Tachi: Enpi
        { 145, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::None, SkillchainIndex::None) }, //Tachi: Hobaku
        { 146, SkillchainAttributes_t(SkillchainIndex::Transfixion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Tachi: Goten
        { 147, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::None, SkillchainIndex::None) }, //Tachi: Kagero
        { 148, SkillchainAttributes_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Tachi: Jinpu
        { 149, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Tachi: Koki
        { 150, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::None) }, //Tachi: Yukikaze
        { 151, SkillchainAttributes_t(SkillchainIndex::Distortion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Tachi: Gekko
        { 152, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Compression, SkillchainIndex::None) }, //Tachi: Kasha
        { 153, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Tachi: Kaiten
        { 154, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Induration, SkillchainIndex::None) }, //Tachi: Rana
        { 155, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Scission, SkillchainIndex::None) }, //Tachi: Ageha
        { 156, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Tachi: Fudo
        { 157, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Compression, SkillchainIndex::None) }, //Tachi: Shoha
        { 158, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Tachi: Suikawari
        { 160, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Shining Strike
        { 161, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Seraph Strike
        { 162, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::None, SkillchainIndex::None) }, //Brainshaker
        { 165, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Skullbreaker
        { 166, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //True Strike
        { 167, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Judgment
        { 168, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Hexa Strike
        { 169, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Compression, SkillchainIndex::None) }, //Black Halo
        { 170, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Randgrith
        { 172, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Flash Nova
        { 174, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Realmrazer
        { 175, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Exudation
        { 176, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Heavy Swing
        { 177, SkillchainAttributes_t(SkillchainIndex::Impaction, SkillchainIndex::None, SkillchainIndex::None) }, //Rock Crusher
        { 178, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Earth Crusher
        { 179, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Starburst
        { 180, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Sunburst
        { 181, SkillchainAttributes_t(SkillchainIndex::Detonation, SkillchainIndex::None, SkillchainIndex::None) }, //Shell Crusher
        { 182, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Impaction, SkillchainIndex::None) }, //Full Swing
        { 184, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Retribution
        { 185, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Gate of Tartarus
        { 186, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Vidohunir
        { 187, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Garland of Bliss
        { 188, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Omniscience
        { 189, SkillchainAttributes_t(SkillchainIndex::Compression, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Cataclysm
        { 191, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Induration, SkillchainIndex::None) }, //Shattersoul
        { 192, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Flaming Arrow
        { 193, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Piercing Arrow
        { 194, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Dulling Arrow
        { 196, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::Detonation) }, //Sidewinder
        { 197, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Blast Arrow
        { 198, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Arching Arrow
        { 199, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Empyreal Arrow
        { 200, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Namas Arrow
        { 201, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Refulgent Arrow
        { 202, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) }, //Jishnu's Radiance
        { 203, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Apex Arrow
        { 208, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Hot Shot
        { 209, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Split Shot
        { 210, SkillchainAttributes_t(SkillchainIndex::Liquefaction, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Sniper Shot
        { 212, SkillchainAttributes_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::Detonation) }, //Slug Shot
        { 213, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Blast Shot
        { 214, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::None, SkillchainIndex::None) }, //Heavy Shot
        { 215, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Detonator
        { 216, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Coronach
        { 217, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Trueflight
        { 218, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Transfixion, SkillchainIndex::None) }, //Leaden Salute
        { 219, SkillchainAttributes_t(SkillchainIndex::Induration, SkillchainIndex::Detonation, SkillchainIndex::Impaction) }, //Numbing Shot
        { 220, SkillchainAttributes_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::None) }, //Wildfire
        { 221, SkillchainAttributes_t(SkillchainIndex::Fusion, SkillchainIndex::Reverberation, SkillchainIndex::None) }, //Last Stand
        { 224, SkillchainAttributes_t(SkillchainIndex::Fragmentation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Exenterator
        { 225, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Distortion, SkillchainIndex::None) }, //Chant du Cygne
        { 226, SkillchainAttributes_t(SkillchainIndex::Gravitation, SkillchainIndex::Scission, SkillchainIndex::None) }, //Requiescat
        { 227, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::None, SkillchainIndex::None) }, //Knights of Rotund
        { 228, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::None, SkillchainIndex::None) }, //Final Paradise
        { 238, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::None) }, //Uriel Blade
        { 239, SkillchainAttributes_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::None) } //Glory Slash
    };

    std::vector<SkillchainInfo_t> mPossibleSkillchains = {
        //T3
        SkillchainInfo_t(SkillchainIndex::Light, SkillchainIndex::Light, SkillchainIndex::Light),
        SkillchainInfo_t(SkillchainIndex::Light, SkillchainIndex::Fragmentation, SkillchainIndex::Fusion),
        SkillchainInfo_t(SkillchainIndex::Light, SkillchainIndex::Fusion, SkillchainIndex::Fragmentation),
        SkillchainInfo_t(SkillchainIndex::Darkness, SkillchainIndex::Darkness, SkillchainIndex::Darkness),
        SkillchainInfo_t(SkillchainIndex::Darkness, SkillchainIndex::Distortion, SkillchainIndex::Gravitation),
        SkillchainInfo_t(SkillchainIndex::Darkness, SkillchainIndex::Gravitation, SkillchainIndex::Distortion),

        //T2
        SkillchainInfo_t(SkillchainIndex::Fusion, SkillchainIndex::Liquefaction, SkillchainIndex::Impaction),
        SkillchainInfo_t(SkillchainIndex::Fusion, SkillchainIndex::Distortion, SkillchainIndex::Fusion),
        SkillchainInfo_t(SkillchainIndex::Gravitation, SkillchainIndex::Detonation, SkillchainIndex::Compression),
        SkillchainInfo_t(SkillchainIndex::Gravitation, SkillchainIndex::Fusion, SkillchainIndex::Gravitation),
        SkillchainInfo_t(SkillchainIndex::Distortion, SkillchainIndex::Transfixion, SkillchainIndex::Scission),
        SkillchainInfo_t(SkillchainIndex::Distortion, SkillchainIndex::Fragmentation, SkillchainIndex::Distortion),
        SkillchainInfo_t(SkillchainIndex::Fragmentation, SkillchainIndex::Induration, SkillchainIndex::Reverberation),
        SkillchainInfo_t(SkillchainIndex::Fragmentation, SkillchainIndex::Gravitation, SkillchainIndex::Fragmentation),


        //T1
        SkillchainInfo_t(SkillchainIndex::Liquefaction, SkillchainIndex::Impaction, SkillchainIndex::Liquefaction),
        SkillchainInfo_t(SkillchainIndex::Liquefaction, SkillchainIndex::Scission, SkillchainIndex::Liquefaction),
        SkillchainInfo_t(SkillchainIndex::Scission, SkillchainIndex::Liquefaction, SkillchainIndex::Scission),
        SkillchainInfo_t(SkillchainIndex::Scission, SkillchainIndex::Detonation, SkillchainIndex::Scission),
        SkillchainInfo_t(SkillchainIndex::Reverberation, SkillchainIndex::Scission, SkillchainIndex::Reverberation),
        SkillchainInfo_t(SkillchainIndex::Reverberation, SkillchainIndex::Transfixion, SkillchainIndex::Reverberation),
        SkillchainInfo_t(SkillchainIndex::Detonation, SkillchainIndex::Scission, SkillchainIndex::Detonation),
        SkillchainInfo_t(SkillchainIndex::Detonation, SkillchainIndex::Impaction, SkillchainIndex::Detonation),
        SkillchainInfo_t(SkillchainIndex::Detonation, SkillchainIndex::Compression, SkillchainIndex::Detonation),
        SkillchainInfo_t(SkillchainIndex::Induration, SkillchainIndex::Reverberation, SkillchainIndex::Induration),
        SkillchainInfo_t(SkillchainIndex::Impaction, SkillchainIndex::Reverberation, SkillchainIndex::Impaction),
        SkillchainInfo_t(SkillchainIndex::Impaction, SkillchainIndex::Induration, SkillchainIndex::Impaction),
        SkillchainInfo_t(SkillchainIndex::Transfixion, SkillchainIndex::Compression, SkillchainIndex::Transfixion),
        SkillchainInfo_t(SkillchainIndex::Compression, SkillchainIndex::Transfixion, SkillchainIndex::Compression),
        SkillchainInfo_t(SkillchainIndex::Compression, SkillchainIndex::Induration, SkillchainIndex::Compression)
    };

public:
    ResonationTracker(IAshitaCore* pAshitaCore);
    ~ResonationTracker();

    void Clear();
    SkillchainIndex GetResult(IAbility* pWeaponskill);
    int GetTimeUntilOpen();
    WindowState GetWindow();
    void HandleActionPacket(uint32_t size, uint8_t* data);

private:
    SkillchainAttributes_t GetSkillchainAttributes(int index);
    int GetTarget();
};

#endif