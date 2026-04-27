#pragma once

struct Slot
{
    uint32_t address;
};

namespace Slots
{
    static constexpr Slot Telescope        = {0x1506B53C};
    static constexpr Slot TingleBottle     = {0x1506B543};
    static constexpr Slot Pictobox         = {0x1506B544};
    static constexpr Slot SpoilsBag        = {0x1506B540};
    static constexpr Slot BaitBag          = {0x1506B547};
    static constexpr Slot DeliveryBag      = {0x1506B54E};

    static constexpr Slot GrapplingHook    = {0x1506B53F};
    static constexpr Slot Boomerang        = {0x1506B541};
    static constexpr Slot SkullHammer      = {0x1506B550};
    static constexpr Slot DekuLeaf         = {0x1506B542};

    static constexpr Slot Bow              = {0x1506B548};
    static constexpr Slot IronBoots        = {0x1506B545};
    static constexpr Slot Bombs            = {0x1506B549};
    static constexpr Slot Hookshot         = {0x1506B54F};
    static constexpr Slot MagicArmor       = {0x1506B546};

    static constexpr Slot Bottle1          = {0x1506B54A};
    static constexpr Slot Bottle2          = {0x1506B54B};
    static constexpr Slot Bottle3          = {0x1506B54C};
    static constexpr Slot Bottle4          = {0x1506B54D};

    static constexpr Slot Sword            = {0x1506B50E};
    static constexpr Slot Shield           = {0x1506B50F};
    static constexpr Slot WindWaker        = {0x1506B53E};
    static constexpr Slot HeroCharm        = {0x1506B5B8};
    static constexpr Slot Sail             = {0x1506B53D};
    static constexpr Slot PowerBracelets   = {0x1506B5B6};

    static constexpr Slot Triforce         = {0x1506B5BE};
    static constexpr Slot Pearls           = {0x1506B5BF};
    static constexpr Slot Songs            = {0x1506B5BD};

    static constexpr Slot BombBag          = {0x1506B570};
    static constexpr Slot Quiver           = {0x1506B56F};
    static constexpr Slot Wallet           = {0x1506B512};
    static constexpr Slot MagicMeter       = {0x1506B513};

    static constexpr Slot Mail1            = {0x1506B586};
    static constexpr Slot Mail2            = {0x1506B587};
    static constexpr Slot Mail3            = {0x1506B588};
    static constexpr Slot Mail4            = {0x1506B589};
    static constexpr Slot Mail5            = {0x1506B58A};
    static constexpr Slot Mail6            = {0x1506B58B};
    static constexpr Slot Mail7            = {0x1506B58C};
    static constexpr Slot Mail8            = {0x1506B58D};
}


static const uint32_t gSlots[] =
{
    Slots::Telescope.address,
    Slots::TingleBottle.address,
    Slots::Pictobox.address,
    Slots::SpoilsBag.address,
    Slots::BaitBag.address,
    Slots::DeliveryBag.address,

    Slots::GrapplingHook.address,
    Slots::Boomerang.address,
    Slots::SkullHammer.address,
    Slots::DekuLeaf.address,

    Slots::Bow.address,
    Slots::IronBoots.address,
    Slots::Bombs.address,
    Slots::Hookshot.address,
    Slots::MagicArmor.address,

    Slots::Bottle1.address,
    Slots::Bottle2.address,
    Slots::Bottle3.address,
    Slots::Bottle4.address,

    Slots::Sword.address,
    Slots::Shield.address,
    Slots::WindWaker.address,
    Slots::HeroCharm.address,
    Slots::Sail.address,
    Slots::PowerBracelets.address,

    Slots::BombBag.address,
    Slots::Quiver.address,
    Slots::Wallet.address,
    Slots::MagicMeter.address
};


#define SLOT_COUNT (sizeof(gSlots) / sizeof(gSlots[0]))