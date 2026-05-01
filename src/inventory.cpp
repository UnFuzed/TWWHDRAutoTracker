#include "inventory.h"
#include "udp_thread.h"
#include "memory_map.h"

#include <stdio.h>

#define TICK_SKIP 6

static uint8_t last[SLOT_COUNT];
static uint32_t tickSkip = 0;

static inline uint8_t safeRead(uint32_t addr)
{
    if (addr < 0x10000000 || addr > 0x1FFFFFFF)
        return 0;

    return *(volatile uint8_t*)addr;
}

static void handleBitset(uint32_t idx, uint8_t prev, uint8_t current, const char* name)
{
    if (prev == current)
        return;

    uint8_t diff = prev ^ current;

    for (int bit = 0; bit < 8; bit++)
    {
        uint8_t mask = (1 << bit);

        // Only detect 0 → 1
        if ((diff & mask) && (current & mask))
        {
            char msg[128];
            snprintf(msg, sizeof(msg),
                     "%s BIT %d OBTAINED (VAL=%02X)",
                     name, bit + 1, current);
            UDP_Send(msg);
        }
    }
}

static void check(uint32_t idx, uint32_t addr)
{
    uint8_t current = safeRead(addr);
    uint8_t prev = last[idx];

    switch (idx)
    {
        case TRIFORCE_IDX:
            handleBitset(idx, prev, current, "TRIFORCE");
            break;

        case PEARLS_IDX:
            handleBitset(idx, prev, current, "PEARL");
            break;

        case SONGS_IDX:
            handleBitset(idx, prev, current, "SONG");
            break;

        default:
        {
            if (prev != current && prev == 0x00 && current != 0x00)
            {
                char msg[128];
                snprintf(msg, sizeof(msg),
                         "ITEM IDX=%u VAL=%02X",
                         idx, current);
                UDP_Send(msg);
            }
            break;
        }
    }

    last[idx] = current;
}

void Inventory_Init()
{
    for (uint32_t i = 0; i < SLOT_COUNT; i++)
    {
        last[i] = 0x00;
    }
}

void Inventory_Tick()
{
    if (++tickSkip < TICK_SKIP)
        return;

    tickSkip = 0;

    for (uint32_t i = 0; i < SLOT_COUNT; i++)
    {
        check(i, gSlots[i]);
    }
}