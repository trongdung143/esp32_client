#pragma once

#include "setup.h"
#include <driver/i2s.h>

void mic_init();
void mic_task(void *param);