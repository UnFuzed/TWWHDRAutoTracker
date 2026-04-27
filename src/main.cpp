#include <wups.h>
#include <coreinit/title.h>
#include <coreinit/thread.h>

#include <wups/config/WUPSConfigCategory.h>
#include <wups/config/WUPSConfigItemIPAddress.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <wups/config_api.h>


#include "inventory.h"
#include "udp_thread.h"

WUPS_PLUGIN_NAME("TWWHDHDR Auto Tracker");
WUPS_PLUGIN_DESCRIPTION("Live UDP Auto Tracker for The Wind Waker HD");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("UnFuzed");
WUPS_PLUGIN_LICENSE("BSD");

#define TWWHD_TITLE_ID 0x0005000010143500ULL //Will be changed to TWWHDR title ID later

#define IP_KEY   "pcIp"
#define PORT_KEY "pcPort"

static uint32_t sPcIp   = 0xC0A832AC; //192.168.50.172
static uint32_t sPcPort = 4444;


static OSThread thread;
static uint8_t stack[0x4000];
static volatile bool running = false;
static bool isConnected = false;

static void ipChanged(ConfigItemIPAddress *item, uint32_t value)
{
    (void)item;
    sPcIp = value;

    WUPSStorageAPI::Store(IP_KEY, sPcIp);
    WUPSStorageAPI::SaveStorage();
}

static void portChanged(ConfigItemIntegerRange *item, int value)
{
    (void)item;
    sPcPort = (uint32_t)value;

    WUPSStorageAPI::Store(PORT_KEY, sPcPort);
    WUPSStorageAPI::SaveStorage();
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    WUPSConfigCategory root(rootHandle);

    WUPSConfigItemHandle ipHandle;
    WUPSConfigItemIPAddress_Create(
        IP_KEY,
        "PC IP Address",
        sPcIp,
        sPcIp,
        &ipChanged,
        &ipHandle
    );

    WUPSConfigAPI_Category_AddItem(rootHandle, ipHandle);

    root.add(WUPSConfigItemIntegerRange::Create(
        PORT_KEY,
        "PC UDP Port",
        4444,
        (int)sPcPort,
        1,
        65535,
        &portChanged
    ));

    root.add(WUPSConfigItemStub::Create(
        std::string("UDP: ") + (isConnected ? "Running" : "Stopped")
    ));

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback()
{
    WUPSStorageAPI::SaveStorage();
}

static int ThreadEntry(int, const char**)
{
    bool udpReady = false;

    Inventory_Init();

    while (running)
    {
        if (!udpReady)
        {
            isConnected = UDP_Init(sPcIp, (uint16_t)sPcPort);
            udpReady = true;
        }

        Inventory_Tick();

        OSSleepTicks(OSMillisecondsToTicks(50));
    }

    if (udpReady)
    {
        UDP_Close();
        isConnected = false;
    }

    return 0;
}

INITIALIZE_PLUGIN()
{
    WUPSConfigAPIOptionsV1 opt{};
    opt.name = "TWWHDR Auto Tracker";

    WUPSConfigAPI_Init(opt, ConfigMenuOpenedCallback, ConfigMenuClosedCallback);

    WUPSStorageAPI::GetOrStoreDefault(IP_KEY, sPcIp, 0xC0A832AC);
    WUPSStorageAPI::GetOrStoreDefault(PORT_KEY, sPcPort, (uint32_t)4444);

    WUPSStorageAPI::SaveStorage();
}

ON_APPLICATION_START()
{
    if (OSGetTitleID() != TWWHD_TITLE_ID) // Only run for TWWHD
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
}