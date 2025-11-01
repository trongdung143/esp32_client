#include "spk.h"
#include "display.h"
#include "utils.h"
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
    (void *)param;
    while (true)
    {
        if (!spk_enabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        PcmChunk chunk;

        if (xQueueReceive(server_to_spk, &chunk, pdMS_TO_TICKS(2000)) == pdTRUE)
        {
            if (chunk.pcm && chunk.bytes > 0)
            {
                size_t bytes_written;

                i2s_write(I2S_SPK_PORT, chunk.pcm, chunk.bytes, &bytes_written, portMAX_DELAY);

                heap_caps_free(chunk.pcm);
            }
        }
        else
            update_state();
    }
}

static void update_state()
{
    spk_enabled = false;
    clear_queue_and_free(server_to_spk);
    i2s_stop(I2S_SPK_PORT);

    vTaskDelay(pdMS_TO_TICKS(100));

    clear_queue_and_free(mic_to_server);
    clear_queue_and_free(temp_is_silent);
    i2s_start(I2S_MIC_PORT);
    mic_enabled = true;

    face_state = 1;
}