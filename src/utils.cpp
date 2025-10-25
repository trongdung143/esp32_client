#include "utils.h"

void clear_queue_and_free(QueueHandle_t queue)
{
    PcmChunk chunk;
    while (xQueueReceive(queue, &chunk, 0) == pdTRUE)
    {
        if (chunk.pcm)
        {
            heap_caps_free(chunk.pcm);
        }
    }
    xQueueReset(queue);
}