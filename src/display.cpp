#include "display.h"

SPIClass spi(HSPI);
Adafruit_GC9A01A tft(&spi, TFT_DC, TFT_CS, TFT_RST);

uint16_t *frame_buf = nullptr;

uint8_t eye_width = 50;
uint8_t eye_height = 50;
uint8_t eye_spacing = 40;

uint8_t left_eye_x = 50;
uint8_t right_eye_x = left_eye_x + eye_width + eye_spacing;
uint8_t eye_y = 70;

uint8_t target_left_eye_x = left_eye_x;
uint8_t target_right_eye_x = right_eye_x;
uint8_t move_speed = 2;

uint8_t blink_state = 0;
uint16_t blink_delay = 2000;
uint64_t last_blink_time = 0;
uint64_t move_time = 0;
uint64_t speak_anim_time = 0;

uint8_t speak_phase = 0;

void display_init()
{
    spi.end();
    spi.begin(TFT_SCL, -1, TFT_SDA, TFT_CS);
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(0);
    tft.fillScreen(GC9A01A_BLACK);

    frame_buf = (uint16_t *)heap_caps_malloc(SCREEN_W * SCREEN_H * 2, MALLOC_CAP_SPIRAM);
}

void display_text(const String &text, uint16_t text_color)
{
    tft.setTextSize(2);
    tft.fillRect(0, 100, 240, 40, GC9A01A_BLACK);
    uint8_t x = 240 / 2 - (text.length() * 12) / 2;
    tft.setCursor(x, 120);
    tft.setTextColor(text_color);
    tft.println(text);
}

void display_clear()
{
    tft.fillScreen(GC9A01A_BLACK);
}

static inline void draw_pixel(int x, int y, uint16_t color)
{
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
        return;
    frame_buf[y * SCREEN_W + x] = color;
}

static void fill_round_rect(int x, int y, int w, int h, int r, uint16_t color)
{
    for (int j = y + r; j < y + h - r; j++)
        for (int i = x; i < x + w; i++)
            draw_pixel(i, j, color);

    for (int j = y; j < y + r; j++)
        for (int i = x + r; i < x + w - r; i++)
            draw_pixel(i, j, color);
    for (int j = y + h - r; j < y + h; j++)
        for (int i = x + r; i < x + w - r; i++)
            draw_pixel(i, j, color);

    for (int j = -r; j <= r; j++)
        for (int i = -r; i <= r; i++)
            if (i * i + j * j <= r * r)
            {
                draw_pixel(x + r + i, y + r + j, color);
                draw_pixel(x + w - r - 1 + i, y + r + j, color);
                draw_pixel(x + r + i, y + h - r - 1 + j, color);
                draw_pixel(x + w - r - 1 + i, y + h - r - 1 + j, color);
            }
}

static void fill_rect(int x, int y, int w, int h, uint16_t color)
{
    for (int j = y; j < y + h; j++)
        for (int i = x; i < x + w; i++)
            draw_pixel(i, j, color);
}

static void draw_eye(int x, int y, int w, int h, bool focus)
{
    int r = focus ? 5 : 10;
    fill_round_rect(x, y, w, h, r, GC9A01A_WHITE);
}

static void draw_mouth(uint8_t state, uint8_t phase)
{
    int x = 94;
    int y = 160;
    int w = 52;
    int h = 8;

    if (state == 0)
    {
        fill_round_rect(x, y, w, h, 3, GC9A01A_WHITE);
    }
    else if (state == 1)
    {
        fill_round_rect(x, y - 2, w, h + 4, 3, GC9A01A_WHITE);
    }
    else if (state == 2)
    {
        int mouth_h = 6 + (phase % 3) * 4;
        fill_round_rect(x, y - mouth_h / 2, w, mouth_h, 3, GC9A01A_WHITE);
    }
}

void display_face_task(void *param)
{
    (void *)param;
    while (true)
    {
        if (!display_face_enabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        uint64_t current_time = millis();

        if (face_state == 0 || face_state == 1)
        {
            if (current_time - last_blink_time > blink_delay && blink_state == 0)
            {
                blink_state = 1;
                last_blink_time = current_time;
            }
            else if (current_time - last_blink_time > 400 && blink_state == 1)
            {
                blink_state = 0;
                last_blink_time = current_time;
            }
        }

        if (current_time - move_time > random(2000, 5000) && blink_state == 0)
        {
            uint8_t eye_move = random(0, 3);
            if (eye_move == 1)
            {
                target_left_eye_x = 45;
                target_right_eye_x = 135;
            }
            else if (eye_move == 2)
            {
                target_left_eye_x = 55;
                target_right_eye_x = 145;
            }
            else
            {
                target_left_eye_x = 50;
                target_right_eye_x = 140;
            }
            move_time = current_time;
        }

        if (left_eye_x != target_left_eye_x)
            left_eye_x += (target_left_eye_x - left_eye_x) / move_speed;
        if (right_eye_x != target_right_eye_x)
            right_eye_x += (target_right_eye_x - right_eye_x) / move_speed;

        memset(frame_buf, 0, SCREEN_W * SCREEN_H * 2);

        if (blink_state == 0)
        {
            bool focus = (face_state == 1);
            draw_eye(left_eye_x, eye_y, eye_width, eye_height, focus);
            draw_eye(right_eye_x, eye_y, eye_width, eye_height, focus);
        }
        else
        {
            fill_rect(left_eye_x, eye_y + eye_height / 2, eye_width, 4, GC9A01A_WHITE);
            fill_rect(right_eye_x, eye_y + eye_height / 2, eye_width, 4, GC9A01A_WHITE);
        }

        if (face_state == 2 && current_time - speak_anim_time > 120)
        {
            speak_phase++;
            speak_anim_time = current_time;
        }

        draw_mouth(face_state, speak_phase);
        tft.drawRGBBitmap(0, 0, frame_buf, SCREEN_W, SCREEN_H);

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
