#include "display.h"

SPIClass spi(HSPI);

Adafruit_GC9A01A tft(&spi, TFT_DC, TFT_CS, TFT_RST);

void display_init()
{
    spi.begin(TFT_SCL, -1, TFT_SDA, TFT_CS);
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(0);
    tft.fillScreen(GC9A01A_BLACK);
    tft.setCursor(30, 120);
    tft.setTextColor(GC9A01A_GREEN);
    tft.setTextSize(2);
    tft.println("GC9A01A OK ============================================================!");
}
