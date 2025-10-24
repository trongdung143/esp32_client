#include "mic.h"

void mic_init()
{
    i2s_config_t cfg = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_MIC_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = I2S_MIC_DMA_BUF_COUNT,
        .dma_buf_len = I2S_MIC_DMA_BUF_LEN,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0};

    i2s_pin_config_t pin = {
        .bck_io_num = I2S_MIC_SCK,
        .ws_io_num = I2S_MIC_WS,
        .data_out_num = -1,
        .data_in_num = I2S_MIC_SD};

    i2s_driver_install(I2S_MIC_PORT, &cfg, 0, NULL);
    i2s_set_pin(I2S_MIC_PORT, &pin);
    i2s_start(I2S_MIC_PORT);
}

void mic_task(void *param)
{
}