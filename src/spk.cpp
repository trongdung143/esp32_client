#include "spk.h"

void spk_init()
{
    i2s_config_t cfg = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = I2S_SPK_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = I2S_SPK_DMA_BUF_COUNT,
        .dma_buf_len = I2S_SPK_DMA_BUF_LEN,
        .use_apll = true};

    i2s_pin_config_t pin = {
        .bck_io_num = I2S_SPK_BCLK,
        .ws_io_num = I2S_SPK_LRCK,
        .data_out_num = I2S_SPK_DOUT,
        .data_in_num = -1};

    i2s_driver_install(I2S_SPK_PORT, &cfg, 0, NULL);
    i2s_set_pin(I2S_SPK_PORT, &pin);
    i2s_start(I2S_SPK_PORT);
}

void spk_task(void *param)
{
}