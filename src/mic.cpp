#include "mic.h"
#include "connection.h"
#include "display.h"
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
    (void)param;

    uint8_t buff_size = 3;
    int32_t i2s_read_buff[I2S_MIC_DMA_BUF_LEN * buff_size];
    int16_t buf[I2S_MIC_DMA_BUF_LEN * buff_size];
    size_t bytes_read;

    while (true)
    {
        if (!mic_enabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        i2s_read(I2S_MIC_PORT, i2s_read_buff, sizeof(i2s_read_buff), &bytes_read, portMAX_DELAY);

        shift_bit(i2s_read_buff, buf, bytes_read / 4);

        if (!is_silent(buf, bytes_read / 4, SILENCE_THRESHOLD))
        {
            int16_t *pcm = (int16_t *)heap_caps_malloc(bytes_read / 2, MALLOC_CAP_SPIRAM);

            memcpy(pcm, buf, bytes_read / 2);
            PcmChunk chunk = {pcm, bytes_read / 2};

            xQueueSend(mic_to_server, &chunk, 0);
            UBaseType_t count = uxQueueMessagesWaiting(mic_to_server);

            update_state(count);
        }
    }
}

void shift_bit(int32_t *i2s_read_buff, int16_t *buf, size_t len)
{
    for (int i = 0; i < len; i++)
        buf[i] = i2s_read_buff[i] >> 14;
}

bool is_silent(int16_t *buf, size_t len, uint16_t threshold)
{
    uint64_t sum = 0;
    for (int i = 0; i < len; i++)
        sum += abs(buf[i]);
    return (sum / len) < threshold;
}

static void update_state(UBaseType_t count)
{
    if (count >= 8 && !pcm_sending)
    {
        ws.sendTXT("start_chat|" + String(CLIENT_ID));
        pcm_sending = true;
    }
}