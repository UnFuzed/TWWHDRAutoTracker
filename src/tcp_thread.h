#pragma once
#include <cstdint>

bool TCP_Init(uint32_t ip, uint16_t port);
void TCP_Send(const char* msg);
void TCP_Close();
