#include "connection.h"
#include "display.h"
#include "utils.h"
#include "spk.h"

extern volatile uint8_t face_state;

WebSocketsClient ws;

void wifi_connect()
{
    WiFiManager wifi_manager;
    display_text("connecting", GC9A01A_ORANGE);
    bool connected = wifi_manager.autoConnect("chatbot_client", "12345678");

    if (connected)
    {
        String ssid = WiFi.SSID();
        display_text("connected to " + ssid, GC9A01A_GREEN);
        ws_connect();
    }
    else
    {
        display_text("no connection", GC9A01A_RED);
    }
}

static void on_ws_event(WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_CONNECTED)
    {
        display_face_enabled = true;

        clear_queue_and_free(server_to_spk);
        clear_queue_and_free(mic_to_server);

        spk_enabled = false;
        pcm_receiving = false;
        pcm_sending = false;
        mic_enabled = true;

        face_state = 1;
    }
    else if (type == WStype_DISCONNECTED)
    {
        display_face_enabled = false;
        display_clear();
        display_text("disconnect-server", GC9A01A_RED);
    }
    else if (type == WStype_TEXT)
    {
        String msg = String((char *)payload);
        if (msg.startsWith("start_stream_audio"))
        {
            pcm_receiving = true;
        }
        else if (msg.startsWith("end_stream_audio"))
        {
            pcm_receiving = false;
        }
    }
    else if (type == WStype_BIN)
    {
        if (pcm_receiving)
        {
            int16_t *pcm = (int16_t *)heap_caps_malloc(length, MALLOC_CAP_SPIRAM);
            if (pcm)
            {
                memcpy(pcm, payload, length);
                PcmChunk chunk = {pcm, length};
                xQueueSend(server_to_spk, &chunk, 0);
                UBaseType_t count = uxQueueMessagesWaiting(server_to_spk);
                if (!spk_enabled && count >= 1)
                {
                    i2s_start(I2S_SPK_PORT);
                    spk_enabled = true;
                    face_state = 2;
                }
            }
        }
    }
    else if (type == WStype_ERROR)
    {
    }
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

void send_pcm_task(void *param)
{
    (void)param;
    while (true)
    {
        if (!pcm_sending)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        PcmChunk chunk;
        if (xQueueReceive(mic_to_server, &chunk, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            if (chunk.pcm && chunk.bytes > 0 && ws.isConnected())
                ws.sendBIN((uint8_t *)chunk.pcm, chunk.bytes);

            if (chunk.pcm)
                heap_caps_free(chunk.pcm);
        }
        else
            update_state();
    }
}

static void update_state()
{
    ws.sendTXT("end_chat");
    face_state = 3;

    pcm_sending = false;
    mic_enabled = false;
    i2s_stop(I2S_MIC_PORT);
}
