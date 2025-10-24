#include "connection.h"

WebSocketsClient ws;

void wifi_connect()
{
    WiFiManager wifi_manager;
    bool connected = wifi_manager.autoConnect("chatbot_client", "12345678");

    if (connected)
    {
    }
    else
    {
    }
}

static void on_ws_event(WStype_t type, uint8_t *payload, size_t length)
{
}
void ws_connect()
{
    String server = IP_SERVER;
    uint16_t port = PORT_SERVER;
    String end_point = END_POINT_AUDIO;

    ws.begin(server, port, end_point);
    ws.onEvent(on_ws_event);
    ws.setReconnectInterval(5000);
    ws.enableHeartbeat(15000, 3000, 2);
}

void ws_loop()
{
    ws.loop();
}