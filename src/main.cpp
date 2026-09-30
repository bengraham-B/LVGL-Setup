#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

static const uint32_t screenWidth  = 320;
static const uint32_t screenHeight = 240;
static lv_color_t buf[screenWidth * screenHeight / 10];

static void my_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

static uint32_t my_tick_cb(void) { return millis(); }

void setup()
{
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    lv_init();
    lv_tick_set_cb(my_tick_cb);

    lv_display_t* disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_flush_cb);
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello LVGL!");
    lv_obj_center(label);
}

void loop()
{
    lv_timer_handler();
    delay(5);
}