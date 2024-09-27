#include "M5CoreInk.h"
#include <WiFi.h>
#include "time.h"

const char* ssid             = "gg";
const char* password         = "gy142857";
const char* ntpServer        = "pool.ntp.org";
const long gmtOffset_sec     = 3600;
const int daylightOffset_sec = 3600;

// every hour at minute 45 do a full ink display refresh
#define FULL_REFRESH_MINUTE (45)

Ink_Sprite InkPageSprite(&M5.M5Ink);  //创建 M5Ink实例

void ButtonPrint(const char* str,uint16_t posX,uint16_t posY) {
    InkPageSprite.clear();                  // clear the screen.  清屏
    InkPageSprite.drawString(posX, posY, str);  // draw the string.  绘制字符串
    InkPageSprite.pushSprite();             // push the sprite.  推送图片
    delay(1000);
}

void setup() {
    M5.begin();
    if (!M5.M5Ink.isInit()) {
        Serial.printf("Ink Init faild");
    }
    M5.M5Ink.clear();  // Clear the screen. 清屏
    delay(1000);
    // creat ink Sprite. 创建图像区域
    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0) {
        Serial.printf("Ink Sprite create faild");
    }
    InkPageSprite.drawString(20, 20, "");
    ButtonPrint("Hello GuYan!",40,90);
}

void loop() {
    if (M5.BtnUP.wasPressed())
        ButtonPrint("Btn UP Pressed",40,30);
    if (M5.BtnDOWN.wasPressed())
        ButtonPrint("Btn DOWN Pressed",40,90);
    if (M5.BtnMID.wasPressed())
        ButtonPrint("Btn MID Pressed",40,60);
    if (M5.BtnEXT.wasPressed())
        ButtonPrint("Btn EXT Pressed",40,5);
    if (M5.BtnPWR.wasPressed()) {  // Right button press.  右侧按键按下
        ButtonPrint("Btn PWR Pressed",40,160);
        M5.shutdown();
    }
    M5.update();  // Refresh device button. 刷新设备按键
}
