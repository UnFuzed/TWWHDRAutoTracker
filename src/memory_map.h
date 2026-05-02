#pragma once
#include <stdint.h>

enum SlotType
{
    SLOT_NORMAL,
    SLOT_BITSET
};

struct Slot
{
    const char* name;
    uint32_t address;
    SlotType type;
    const char** bitNames;
};

#define SLOT(name, addr)        { name, addr, SLOT_NORMAL, nullptr }
#define BITSET(name, addr, bits){ name, addr, SLOT_BITSET, bits }


static const char* TRIFORCE_NAMES[8] =
{
    "Shard 1","Shard 2","Shard 3","Shard 4",
    "Shard 5","Shard 6","Shard 7","Shard 8"
};

static const char* PEARL_NAMES[8] =
{
    "Nayru's Pearl", "Farore's Pearl", "Din's Pearl"
};

static const char* SONG_NAMES[8] =
{
    "Wind's Requiem", "Ballad of Gales", "Command Melody",
    "Earth God's Lyric", "Wind God's Aria", "Song of Passing",
};

static const Slot gSlots[] =
{
    SLOT("Telescope",      0x1506B53C),
    SLOT("TingleBottle",   0x1506B543),
    SLOT("Pictobox",       0x1506B544),
    SLOT("SpoilsBag",      0x1506B540),
    SLOT("BaitBag",        0x1506B547),
    SLOT("DeliveryBag",    0x1506B54E),

    SLOT("MailBag1", 0x1506B586),
    SLOT("MailBag2", 0x1506B587),
    SLOT("MailBag3", 0x1506B588),
    SLOT("MailBag4", 0x1506B589),
    SLOT("MailBag5", 0x1506B58A),
    SLOT("MailBag6", 0x1506B58B),
    SLOT("MailBag7", 0x1506B58C),
    SLOT("MailBag8", 0x1506B58D),

    SLOT("GrapplingHook",  0x1506B53F),
    SLOT("Boomerang",      0x1506B541),
    SLOT("SkullHammer",    0x1506B550),
    SLOT("DekuLeaf",       0x1506B542),

    SLOT("Bow",            0x1506B548),
    SLOT("IronBoots",      0x1506B545),
    SLOT("Bombs",          0x1506B549),
    SLOT("Hookshot",       0x1506B54F),
    SLOT("MagicArmor",     0x1506B546),

    SLOT("Bottle1",        0x1506B54A),
    SLOT("Bottle2",        0x1506B54B),
    SLOT("Bottle3",        0x1506B54C),
    SLOT("Bottle4",        0x1506B54D),

    SLOT("Sword",          0x1506B50E),
    SLOT("Shield",         0x1506B50F),
    SLOT("WindWaker",      0x1506B53E),
    SLOT("HeroCharm",      0x1506B5B8),
    SLOT("Sail",           0x1506B53D),
    SLOT("PowerBracelets", 0x1506B5B6),

    SLOT("BombBag",        0x1506B570),
    SLOT("Quiver",         0x1506B56F),
    SLOT("Wallet",         0x1506B512),
    SLOT("MagicMeter",     0x1506B513),

    BITSET("Triforce", 0x1506B5BE, TRIFORCE_NAMES),
    BITSET("Pearls",   0x1506B5BF, PEARL_NAMES),
    BITSET("Songs",    0x1506B5BD, SONG_NAMES),
};

#define SLOT_COUNT (sizeof(gSlots) / sizeof(gSlots[0]))