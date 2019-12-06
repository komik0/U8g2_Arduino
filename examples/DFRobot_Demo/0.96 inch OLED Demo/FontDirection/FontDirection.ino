/*!
 * @file Font.ino
 * @brief U8G2中支持的几种字体方向的显示
 * @n U8G2支持多种字体，此demo只展示了四种方向（没有展示镜像）。
 * @n U8G2字体GitHub连接：https://github.com/olikraus/u8g2/wiki/fntlistall
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
 *IIC构造函数
 *@param 指定大局显示的基本布局：	U8G2_R0 不旋转，横向，绘制方向从左到右
									U8G2_R1 顺时针旋转90度，绘制方向从上到下
									U8G2_R2 顺时针旋转180度，绘制方向从右到左
									U8G2_R3 顺时针旋转270度，绘制方向从下到上
									U8G2_MIRROR 正常显示镜像内容（v2.6.x版本以上使用)   注意:U8G2_MIRROR需要与setFlipMode（）配搭使用.
 *@param reset：U8x8_PIN_NONE 表示引脚为空，不会使用复位引脚
 *SPI构造函数
 *@param  cs 按引脚接上即可（引脚可自己选择）
 *@param  dc 按引脚接上即可（引脚可自己选择）
 *
*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //  M0/ESP32/ESP8266/mega2560/Uno/Leonardo
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9);


//width:30,height:30 
const uint8_t col[] U8X8_PROGMEM= {0x00,0xc0,0x00,0x00,0x00,0xe0,0x01,0x00,0x00,0xe0,0x01,0x00,0x00,
                                         0xc0,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0xe0,
                                         0x01,0x00,0x00,0xf8,0x07,0x00,0x06,0xfe,0x1f,0x18,0x07,0xff,0x3f,
                                         0x38,0xdf,0xff,0xff,0x3e,0xfa,0xff,0xff,0x17,0xf0,0xff,0xff,0x03,
                                         0xe0,0xff,0xff,0x01,0xe0,0xff,0xff,0x01,0xe0,0xff,0xff,0x01,0xe0,
                                         0xff,0xff,0x01,0x20,0x00,0x00,0x01,0xa0,0xff,0x7f,0x01,0xa0,0x01,
                                         0x60,0x01,0x20,0x07,0x38,0x01,0xe0,0x0c,0xcc,0x01,0xe0,0x39,0xe7,
                                         0x01,0xe0,0xe7,0xf9,0x01,0xc0,0x1f,0xfe,0x00,0x80,0xff,0x7f,0x00,
                                         0x00,0xfe,0x1f,0x00,0x00,0xf8,0x07,0x00,0x00,0xe0,0x01,0x00,0x00,
                                         0xc0,0x00,0x00}; 

void setup() {
  u8g2.begin();
  u8g2.setFontPosTop();/*使用drawStr显示字符串时，默认标准为显示字符的左下角坐标。
                          本函数的功能可理解为将坐标位置改为显示字符串的左上角为坐标标准。*/
}

void Rotation() {
  u8g2.setFont(u8g2_font_bracketedbabies_tr        );
  u8g2.firstPage(); 
  do {
    u8g2.drawXBMP( /* x=*/0 , /* y=*/0 , /* width=*/30 , /* height=*/30 , col );//绘制图像
	/*@brief 设置所有字符串或字形的绘制方向setFontDirection(uint8_t dir)
     *@param dir=0，旋转0度
		     dir=1，旋转90度
			 dir=2，旋转180度
			 dir=3，旋转270度
    */
    u8g2.setFontDirection(0);			
    u8g2.drawStr( /* x=*/64,/* y=*/32, " DFR");		//绘制字符串
    u8g2.setFontDirection(1);
    u8g2.drawStr(64,32, " DFR");
    u8g2.setFontDirection(2);
    u8g2.drawStr(64,32, " DFR");
    u8g2.setFontDirection(3);
    u8g2.drawStr(64,32, " DFR");
  }while(u8g2.nextPage()); 
  delay(1500);
}

void loop(void)
{ 
    u8g2.setFont( u8g2_font_sticker_mel_tr      );
    for(int i = 0 ; i < 4 ; i++ )
    {
       switch(i)
       {
          case 0:
            u8g2.setFontDirection(0);
            break;
          case 1:
            u8g2.setFontDirection(1);
            break;
          case 2:
            u8g2.setFontDirection(2);
            break;
          case 3: 
            u8g2.setFontDirection(3);
            break;
       }
	  /*
	   * firstPage方法会把当前页码位置变成0
	   * 修改内容处于firstPage和nextPage之间，每次都是重新渲染所有内容
	   * 该方法消耗的ram空间，比sendBuffer消耗的ram空间要少
	  */ 
      u8g2.firstPage();  
      do 
      {
        u8g2.drawStr(64, 32, "DFR");
        u8g2.drawXBMP( 0 , 0 , 30 , 30 , col );
      }while(u8g2.nextPage()); 
      delay(500);
    }
    Rotation();
}