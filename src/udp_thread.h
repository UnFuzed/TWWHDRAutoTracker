#pragma once

bool UDP_Init(uint32_t ip, uint16_t port);
void UDP_Send(const char* msg);
void UDP_Close();