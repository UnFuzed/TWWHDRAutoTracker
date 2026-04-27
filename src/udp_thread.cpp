#include "udp_thread.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

static int sock = -1;
static sockaddr_in target;

bool UDP_Init(uint32_t ip, uint16_t port)
{
    if (sock >= 0)
        return true;  // Already initialized

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
        return false;  // Socket creation failed

    target = {};
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    target.sin_addr.s_addr = htonl(ip);

    return (ip != 0 && port != 0);
}

void UDP_Send(const char* msg)
{
    if (sock < 0 || !msg)
        return;

    sendto(sock,
           msg,
           strlen(msg),
           0,
           (sockaddr*)&target,
           sizeof(target));
}

void UDP_Close()
{
    if (sock >= 0)
    {
        close(sock);
        sock = -1;
    }
}