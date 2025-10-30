#include "setup.h"
#include "connection.h"
#include "mic.h"
#include "display.h"
#include "led.h"
#include "spk.h"
#include "touch.h"

QueueHandle_t mic_to_server;
QueueHandle_t server_to_spk;
volatile bool mic_enabled = false;
volatile bool pcm_sending = false;
volatile bool spk_enabled = false;
volatile bool pcm_receiving = false;

void setup()
{
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  mic_to_server = xQueueCreate(50, sizeof(PcmChunk));
  server_to_spk = xQueueCreate(500, sizeof(PcmChunk));
  display_init();
  led_init();
  mic_init();
  spk_init();
  wifi_connect();

  xTaskCreatePinnedToCore(mic_task, "mic_task", 1024 * 20, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(send_pcm_task, "send_pcm_task", 1024 * 12, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(spk_task, "spk_task", 1024 * 12, NULL, 1, NULL, 0);
}

void loop()
{
  ws_loop();
  detected();
  delay(1);
}
