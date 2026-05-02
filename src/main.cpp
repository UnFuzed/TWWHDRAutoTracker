#include <wups.h>
#include <coreinit/title.h>
#include <coreinit/thread.h>

#include <wups/config/WUPSConfigCategory.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <wups/config_api.h>

#include "inventory.h"
#include "tcp_thread.h"

WUPS_PLUGIN_NAME("TWWHDHDR Auto Tracker");
WUPS_PLUGIN_DESCRIPTION("Live TCP Auto Tracker for The Wind Waker HD");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("UnFuzed");
WUPS_PLUGIN_LICENSE("BSD");

#define TWWHD_TITLE_ID 0x0005000010143500ULL

static OSThread thread;
static uint8_t stack[0x4000];
static volatile bool running = false;
static bool isConnected = false;

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    WUPSConfigCategory root(rootHandle);

    root.add(WUPSConfigItemStub::Create(
        std::string("TCP: ") + (isConnected ? "Running" : "Stopped")
    ));

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback()
{
    // Nothing to do here
}

static int ThreadEntry(int, const char**)
{
    bool tcpReady = false;

    Inventory_Init();

    while (running)
    {
        if (!tcpReady)
        {
            isConnected = TCP_Init(0, 4444);
            tcpReady = true;
        }

        Inventory_Tick();
        OSSleepTicks(OSMillisecondsToTicks(50));
    }

    if (tcpReady)
    {
        TCP_Close();
        isConnected = false;
    }

    return 0;
}

INITIALIZE_PLUGIN()
{
    WUPSConfigAPIOptionsV1 opt{};
    opt.name = "TWWHDR Auto Tracker";

    WUPSConfigAPI_Init(opt, ConfigMenuOpenedCallback, ConfigMenuClosedCallback);
}

ON_APPLICATION_START()
{
    if (OSGetTitleID() != TWWHD_TITLE_ID)
        return;

    running = true;

    OSCreateThread(
        &thread,
        ThreadEntry,
        0,
        nullptr,
        stack + sizeof(stack),
        sizeof(stack),
        16,
        0
    );

    OSResumeThread(&thread);
}

ON_APPLICATION_ENDS()
{
    running = false;
    isConnected = false;
    TCP_Close();
}