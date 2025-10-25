#include "display.h"

SPIClass spi(HSPI);

Adafruit_GC9A01A tft(&spi, TFT_DC, TFT_CS, TFT_RST);

void display_init()
{
    spi.end();
    spi.begin(TFT_SCL, -1, TFT_SDA, TFT_CS);
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(0);
    tft.fillScreen(GC9A01A_BLACK);
    tft.setTextSize(2);
    display_text("esp32-client", GC9A01A_WHITE);
}

void display_text(const String &text, uint16_t text_color)
{
    tft.fillRect(0, 100, 240, 40, GC9A01A_BLACK);
    uint8_t x = 240 / 2 - (text.length() * 12) / 2;
    tft.setCursor(x, 120);
    tft.setTextColor(text_color);
    tft.println(text);
}