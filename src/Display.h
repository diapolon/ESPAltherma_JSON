#ifndef Display_h
#define Display_h

#include <lvgl.h>
extern "C" {
   #include <ui.h>
}

LGFX display;

// Mutex for LVGL
SemaphoreHandle_t lvgl_mutex_handle;

void lvgl_lock_init() {
    lvgl_mutex_handle = xSemaphoreCreateMutex();
}

bool lvgl_lock(uint32_t ticks_to_wait) {
    return (xSemaphoreTake(lvgl_mutex_handle, (TickType_t)ticks_to_wait) == pdTRUE);
}

void lvgl_unlock() {
    xSemaphoreGive(lvgl_mutex_handle);
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  display.startWrite();
  display.setAddrWindow(area->x1, area->y1, w, h);
  //display.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  display.pushImage(area->x1, area->y1, w, h, (lgfx::rgb565_t*)color_p);
  display.endWrite();

  lv_disp_flush_ready(disp);
}

void updateScreenInfos(String text) {            
    lv_label_set_text(ui_base_lbSysInfo, (String(FIRMWARE_VERSION) +" - "+ text).c_str());     
}    

void updateScreenMainText(String text) {        
    if (lvgl_lock(portMAX_DELAY)) {
        // Update LVGL objects here
        lv_label_set_text(ui_base_lbMain, text.c_str());    
        lvgl_unlock();
    }
}

void initDisplay() {
    // Init Display    
    display.begin();  
    display.setRotation(SCREEN_ROTATION);// 0 landscape, 1 portrait, ecc.
    //display.invertDisplay(false);       
    display.fillScreen(TFT_WHITE);

    delay(500);
        
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    lv_init();
    
    static lv_color_t buf[SCREEN_WIDTH * SCREEN_HEIGHT / SCREEN_DIVIDER];
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT / SCREEN_DIVIDER);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = display.width();
    disp_drv.ver_res = display.height();
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
        
    lvgl_lock_init();

    ui_init();   
    
    updateScreenInfos("Avvio in corso...");                 
    updateScreenMainText("Daikin Altherma ESP32");                     
}  

void displayTask(void *pvParameters) {
    while(1) {               
        if (lvgl_lock(portMAX_DELAY)) {
            lv_task_handler();
            lvgl_unlock();
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
  
#endif // Display_h