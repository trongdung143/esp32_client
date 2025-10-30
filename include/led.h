#pragma once
#include "setup.h"
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel led;

void led_init();
void led_show(uint8_t r, uint8_t g, uint8_t b);