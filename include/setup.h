#pragma once
#include <Arduino.h>
#include "freertos/queue.h"
#include "esp_heap_caps.h"

extern QueueHandle_t mic_to_server;
extern QueueHandle_t server_to_spk;
extern volatile bool mic_enabled;
extern volatile bool spk_enabled;
extern volatile bool pcm_sending;
extern volatile bool pcm_receiving;

struct PcmChunk
{
    int16_t *pcm;
    size_t bytes;
};
// MIC (INMP441 hoặc tương tự)
#define I2S_MIC_PORT I2S_NUM_0
#define I2S_MIC_SCK 5
#define I2S_MIC_WS 4
#define I2S_MIC_SD 6
#define I2S_MIC_SAMPLE_RATE 16000
#define I2S_MIC_DMA_BUF_LEN 512
#define I2S_MIC_DMA_BUF_COUNT 4
#define READ_INTERVAL 32
#define SILENCE_THRESHOLD 300

// LOA (MAX98357A hoặc tương tự)
#define I2S_SPK_PORT I2S_NUM_1
#define I2S_SPK_BCLK 15
#define I2S_SPK_LRCK 16
#define I2S_SPK_DOUT 7
#define I2S_SPK_SAMPLE_RATE 16000
#define I2S_SPK_DMA_BUF_LEN 512
#define I2S_SPK_DMA_BUF_COUNT 6
#define WRITE_INTERVAL 32

// MÀN HÌNH (GC9A01A – SPI)
#define TFT_CS 41
#define TFT_DC 40
#define TFT_RST 42
#define TFT_SDA 47
#define TFT_SCL 21
#define TFT_BL -1
#define SPI_FREQUENCY 40000000

// LED
#define LED_PIN 48
#define LED_COUNT 1

// WebSocket
#define IP_SERVER "192.168.1.100"
#define PORT_SERVER 8080
#define END_POINT_AUDIO "/ws_audio"
#define CLIENT_ID "c7b12d5f-8a3e-4bc1-ae3d-9f6a1200a52b"
