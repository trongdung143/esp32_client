#include <Arduino.h>
#include "display.h"
#include "led.h"

void setup()
{
  Serial.begin(115200);
  display_init();
  led_init();
}

void loop()
{
}
