#pragma once

#include "setup.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

extern SPIClass spi;
extern Adafruit_GC9A01A tft;

void display_init();
