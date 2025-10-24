#include "led.h"

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void led_init()
{
    led.begin();
    led.show();
    led.setPixelColor(0, led.Color(255, 255, 255));
    led.show();
}
