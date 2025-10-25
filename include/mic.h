#pragma once

#include "setup.h"
#include <driver/i2s.h>

void mic_init();
void mic_task(void *param);
bool is_silent(int16_t *buf, size_t len, uint16_t threshold);
void shift_bit(int32_t *i2s_read_buff, int16_t *buf, size_t len);
static void update_state(UBaseType_t count);