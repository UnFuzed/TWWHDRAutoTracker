#include "inventory.h"
#include "memory_map.h"
#include "udp_thread.h"

#include <stdio.h>
#include <stdint.h>

#define TICK_SKIP 6

static uint8_t last[SLOT_COUNT];
static uint32_t tickSkip = 0;

static inline uint8_t safeRead(uint32_t addr)
{
    if (addr < 0x10000000 || addr > 0x1FFFFFFF)
        return 0;

    return *(volatile uint8_t*)addr;
}

static void check(uint32_t idx, uint32_t addr)
{
    uint8_t current = safeRead(addr);

    if (last[idx] != current && last[idx] == 0x00 && current != 0x00)
    {
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "ITEM IDX=%u VAL=%02X",
                 idx, current);
        UDP_Send(msg);
    }

    last[idx] = current;
}

void Inventory_Init()
{
    for (uint32_t i = 0; i < SLOT_COUNT; i++)
        last[i] = 0xFF;
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