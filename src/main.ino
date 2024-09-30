#include "M5CoreInk.h"
#include <WiFi.h>
#include "time.h"

const char *ssid[2] = {"gg", "gdjk"};
const char *password[2] = {"gy142857", "12345678"};
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// every hour at minute 45 do a full ink display refresh
#define FULL_REFRESH_MINUTE (45)

#define WINDOWSNUM 2
#define NAVIGATION_MIDDLE_POS_X (uint16_t)10
#define NAVIGATION_MIDDLE_POS_Y (uint16_t)100
#define NAVIGATION_WIDTH (uint16_t)8
#define NAVIGATION_HEIGHT (uint16_t)20
#define NAVIGATION_BORDER_WIDTH (uint16_t)6
#define NAVIGATION_START_POS_X (NAVIGATION_MIDDLE_POS_X - NAVIGATION_WIDTH / 2)
#define NAVIGATION_START_POS_Y (NAVIGATION_MIDDLE_POS_Y -            \
                                NAVIGATION_HEIGHT * WINDOWSNUM / 2 - \
                                NAVIGATION_BORDER_WIDTH * (WINDOWSNUM - 1) / 2)

Ink_Sprite InkPageSprite(&M5.M5Ink); // 创建 M5Ink实例
int curWindow = 0;

void drawNavigation(uint16_t index)
{
    for (uint16_t i = 0; i < WINDOWSNUM; i++)
    {
        uint16_t curStartX = NAVIGATION_START_POS_X;
        uint16_t curStartY = NAVIGATION_START_POS_Y +
                             i * (NAVIGATION_BORDER_WIDTH + NAVIGATION_HEIGHT);
        // Serial.printf("%d,%d\n", curStartX, curStartY);
        if (i != index)
        {
            InkPageSprite.drawRect(
                curStartX, curStartY,
                NAVIGATION_BORDER_WIDTH, NAVIGATION_HEIGHT,
                0xFFFF);
        }
        else
        {
            InkPageSprite.fillRect(
                curStartX, curStartY,
                NAVIGATION_BORDER_WIDTH, NAVIGATION_HEIGHT,
                0xFFFF);
        }
    }
}

void ButtonPrint(const char *str, uint16_t posX, uint16_t posY)
{
    InkPageSprite.clear(); // clear the screen.  清屏
    drawNavigation(curWindow);
    InkPageSprite.drawString(posX, posY, str); // draw the string.  绘制字符串
    InkPageSprite.pushSprite();                // push the sprite.  推送图片
    delay(1000);
}

void setup()
{
    M5.begin();
    if (!M5.M5Ink.isInit())
    {
        Serial.printf("Ink Init faild");
    }
    M5.M5Ink.clear(); // Clear the screen. 清屏
    delay(1000);
    // creat ink Sprite. 创建图像区域
    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
    {
        Serial.printf("Ink Sprite create faild");
    }
    Serial.printf("init");
    InkPageSprite.clear();
    InkPageSprite.fillRect(50, 50, 50, 50, 0xFFFF);
    InkPageSprite.pushSprite();
}

void loop()
{
    if (M5.BtnUP.wasPressed())
    {
        curWindow = (curWindow <= 0) ? 4 : (curWindow - 1);
        ButtonPrint("Btn UP Pressed", 40, 30);
    }
    if (M5.BtnDOWN.wasPressed())
    {
        curWindow = (curWindow >= 4) ? 0 : (curWindow + 1);
        ButtonPrint("Btn DOWN Pressed", 40, 90);
    }
    if (M5.BtnMID.wasPressed())
        ButtonPrint("Btn MID Pressed", 40, 60);
    if (M5.BtnEXT.wasPressed())
        ButtonPrint("Btn EXT Pressed", 40, 5);
    if (M5.BtnPWR.wasPressed())
    { // Right button press.  右侧按键按下
        ButtonPrint("Btn PWR Pressed", 40, 160);
        M5.shutdown();
    }
    M5.update(); // Refresh device button. 刷新设备按键
}
