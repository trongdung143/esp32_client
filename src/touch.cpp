#include "touch.h"
#include "led.h"

void detected()
{
    int state = digitalRead(TOUCH_PIN);
    if (state == HIGH)
    {
        led_show(255, 0, 0);
    }
    else
    {
        led_show(0, 255, 0);
    }
}