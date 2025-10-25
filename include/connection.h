#pragma once

#include "setup.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h>

extern WebSocketsClient ws;

void wifi_connect();
void ws_connect();
void ws_loop();
static void on_ws_event(WStype_t type, uint8_t *payload, size_t length);
void send_pcm_task(void *param);
static void update_state();