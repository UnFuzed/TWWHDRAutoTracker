#include "tcp_thread.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>

static int listenSock = -1;
static int connSock = -1;

bool TCP_Init(uint32_t ip, uint16_t port)
{
    if (listenSock >= 0)
        return true;  // Already initialized

    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock < 0)
        return false;  // Socket creation failed

    // Set SO_REUSEADDR
    int reuse = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(listenSock);
        listenSock = -1;
        return false;
    }

    if (listen(listenSock, 1) < 0) {
        close(listenSock);
        listenSock = -1;
        return false;
    }

    // Make socket non-blocking
    fcntl(listenSock, F_SETFL, O_NONBLOCK);

    return (port != 0);
}

void TCP_Send(const char* msg)
{
    if (!msg)
        return;

    // Try to accept a connection if we don't have one
    if (connSock < 0 && listenSock >= 0)
    {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        connSock = accept(listenSock, (sockaddr*)&clientAddr, &clientLen);
        // If no connection available, just return
        if (connSock < 0)
            return;
    }

    if (connSock >= 0)
    {
        send(connSock, msg, strlen(msg), 0);
    }
}

void TCP_Close()
{
    if (connSock >= 0)
    {
        close(connSock);
        connSock = -1;
    }

    if (listenSock >= 0)
    {
        close(listenSock);
        listenSock = -1;
    }
}
