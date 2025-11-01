#pragma once

#include "setup.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

extern SPIClass spi;
extern Adafruit_GC9A01A tft;

#define SCREEN_W 240
#define SCREEN_H 240

void display_init();
void display_text(const String &text, uint16_t text_color);
static inline void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
static void fill_round_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
static void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
static void draw_eye(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool focus);
static void draw_mouth(uint8_t state, uint8_t phase);
void display_face_task(void *param);
void display_clear();