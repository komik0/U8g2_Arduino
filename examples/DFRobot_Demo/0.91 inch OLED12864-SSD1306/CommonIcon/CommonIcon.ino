/*!
 * @file CommonIcon.ino
 * @brief U8G2中支持的几种常见图标的显示
 * @n U8G2支持多种大小的图标，此demo选取几种进行显示
 * @n U8G2图标GitHub连接：https://github.com/olikraus/u8g2/wiki/fntlistall
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Ivey](Ivey.lu@dfrobot.com)
 * @version  V1.0
 * @date  2019-11-29
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/U8g2_Arduino
*/

#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
#include <Wire.h>

/*
 * 显示屏硬件IIC接口构造函数
 *@param rotation：U8G2_R0 不旋转，横向，绘制方向从左到右
		   U8G2_R1 顺时针旋转90度，绘制方向从上到下
		   U8G2_R2 顺时针旋转180度，绘制方向从右到左
		   U8G2_R3 顺时针旋转270度，绘制方向从下到上
		   U8G2_MIRROR 正常显示镜像内容（v2.6.x版本以上使用)   注意:U8G2_MIRROR需要与setFlipMode（）配搭使用.
 *@param reset：U8x8_PIN_NONE 表示引脚为空，不会使用复位引脚
 * 显示屏硬件SPI接口构造函数
 *@param  cs 按引脚接上即可（引脚可自己选择）
 *@param  dc 按引脚接上即可（引脚可自己选择）
 *
*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(/* rotation=*/U8G2_R0, /* reset=*/ U8X8_PIN_NONE);    //  M0/ESP32/ESP8266/mega2560/Uno/Leonardo
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(/* rotation=*/U8G2_R0, /* cs=*/ 10, /* dc=*/ 9);


void setup(void)
{
  u8g2.begin();
  u8g2.setFontPosTop();  /*使用drawStr显示字符串时，默认标准为显示字符的左下角坐标。
                          本函数的功能可理解为将坐标位置改为显示字符串的左上角为坐标标准。*/
}


/*
 * 选择字集
 * u8g2_font_open_iconic_all_1x_t :Width :8  Height :8  encoding : 64-286
 * u8g2_font_open_iconic_all_2x_t :Width :16  Height :16  encoding : 64-286
 * u8g2_font_open_iconic_all_4x_t :Width :32  Height :32  encoding : 64-286
 * u8g2_font_open_iconic_all_6x_t :Width :48  Height :48  encoding : 64-286
 * u8g2_font_open_iconic_all_8x_t :Width :64  Height :64  encoding : 64-286
*/

/*
 * all:  app  arrow  check  email  embedded  gui  human  other  play  text  thing  weather  www
 * all具体化后每次的encoding都是从64开始，数量不定(这样可以节约内存)
 *想要知道每个图标的具体encoding值，可以通过网址https://github.com/olikraus/u8g2/wiki/fntlistall查询icon进行查看
*/
void loop(void)
{
  /*
   * firstPage方法会把当前页码位置变成0
   * 修改内容处于firstPage和nextPage之间，每次都是重新渲染所有内容
   * 该方法消耗的ram空间，比sendBuffer消耗的ram空间要少
  */ 
   u8g2.firstPage();
   for(int i = 64 ; i <287; i++){
   u8g2.clear();
   do
   {
      u8g2.setFont(u8g2_font_open_iconic_all_4x_t );
      u8g2.drawGlyph(/* x=*/0, /* y=*/16, /* encoding=*/i);  //drawGlyph(u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding);
      u8g2.drawGlyph(/* x=*/48, /* y=*/16, /* encoding=*/i+1);  
      u8g2.drawGlyph(/* x=*/96, /* y=*/16, /* encoding=*/i+2);  
   } while ( u8g2.nextPage() );
   i = i+3;
   delay(2000);
  }
}


