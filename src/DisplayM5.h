#ifndef DisplayM5_h
#define DisplayM5_h

#include <M5Unified.h>

int x_center = 0;
int y_center = 0;

void updateScreenInfos(String text) {   
    if (M5.Lcd.getCursorY()+13>M5.Lcd.height()) {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0,0);
    }
    M5.Lcd.drawString(text, 0, 0);    
}    

void updateScreenMainText(String text) {        
    if (M5.Lcd.getCursorY()+13>M5.Lcd.height()) {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0,0);
    }
    M5.Lcd.drawString(text, x_center, y_center);    
}

void initDisplay() {
    auto cfg = M5.config();

    cfg.serial_baudrate = 115200;  // default=115200. if "Serial" is not needed, set it to 0.
    cfg.clear_display = true;      // default=true. clear the screen when begin.
    cfg.output_power = true;       // default=true. use external port 5V output.
    cfg.internal_imu = true;       // default=true. use internal IMU.
    cfg.internal_rtc = true;       // default=true. use internal RTC.
    cfg.internal_spk = false;       // default=true. use internal speaker.
    cfg.internal_mic = false;      // default=true. use internal microphone.
    cfg.led_brightness = 0;        // default= 0. system LED brightness (0=off / 255=max) (※ not NeoPixel)

    M5.begin(cfg);

    x_center = M5.Lcd.width() / 2;
    y_center = M5.Lcd.height() / 2;
    
    M5.Lcd.setRotation(1);
    M5.Lcd.setBrightness(127);    
    M5.Lcd.fillScreen(TFT_BLACK);
    //M5.Lcd.setFont(&FreeSansBold12pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(TFT_WHITE);

    //M5.Display.wakeup();

    updateScreenInfos("Avvio in corso...");                 
    updateScreenMainText("Daikin Altherma ESP32");                     
}  

void displayTask(void *pvParameters) {
    while(1) {               
        M5.update();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
  
#endif // DisplayM5_h