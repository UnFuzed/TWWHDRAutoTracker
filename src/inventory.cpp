#include "inventory.h"
#include "tcp_thread.h"
#include "memory_map.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TICK_SKIP 6

static uint8_t last[SLOT_COUNT];
static uint32_t tickSkip = 0;

static char cachedStage[64] = {0};
static int cachedBlock = 0;

static inline uint8_t safeRead(uint32_t addr)
{
    if (addr < 0x10000000 || addr > 0x1FFFFFFF)
        return 0;

    return *(volatile uint8_t*)addr;
}

static void UpdateStageCache()
{
    const volatile char* stageName = (const volatile char*)0x109763E4;

    if (!stageName)
        return;

    char buffer[64] = {0};

    for (int i = 0; i < 63; i++)
    {
        char c = stageName[i];
        buffer[i] = c;

        if (c == '\0')
            break;
    }

    if (buffer[0] == '\0')
        return;

    if (strcmp(buffer, cachedStage) != 0)
    {
        strncpy(cachedStage, buffer, sizeof(cachedStage) - 1);
        cachedStage[63] = '\0';

        cachedBlock =
            (strcmp(buffer, "sea_T") == 0 ||
             strcmp(buffer, "Name") == 0);
    }
}

static int CheckStage()
{
    return cachedBlock;
}

static void check(uint32_t idx)
{
    const Slot& slot = gSlots[idx];

    uint8_t current = safeRead(slot.address);
    uint8_t prev = last[idx];

    if (current == prev)
        return;

    if (slot.type == SLOT_BITSET)
    {
        uint8_t diff = prev ^ current;

        for (int bit = 0; bit < 8; bit++)
        {
            uint8_t mask = (1 << bit);

            if (diff & mask)
            {
                bool nowOn = (current & mask);

                char msg[128];

                if (slot.bitNames)
                {
                    snprintf(msg, sizeof(msg),
                             "%s: %s %s (VAL=%02X)\n",
                             slot.name,
                             slot.bitNames[bit],
                             nowOn ? "ON" : "OFF",
                             current);
                }
                else
                {
                    snprintf(msg, sizeof(msg),
                             "%s BIT %d %s (VAL=%02X)\n",
                             slot.name,
                             bit + 1,
                             nowOn ? "ON" : "OFF",
                             current);
                }

                TCP_Send(msg);
            }
        }
    }
    else
    {
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "%s CHANGED %02X -> %02X\n",
                 slot.name,
                 prev,
                 current);

        TCP_Send(msg);
    }

    last[idx] = current;
}

void Inventory_Init()
{
    for (uint32_t i = 0; i < SLOT_COUNT; i++)
        last[i] = 0x00;

    memset(cachedStage, 0, sizeof(cachedStage));
}

void Inventory_Tick()
{
    UpdateStageCache();

    if (CheckStage() == 1)
        return;

    if (++tickSkip < TICK_SKIP)
        return;

    tickSkip = 0;

    for (uint32_t i = 0; i < SLOT_COUNT; i++)
    {
        check(i);
    }
}