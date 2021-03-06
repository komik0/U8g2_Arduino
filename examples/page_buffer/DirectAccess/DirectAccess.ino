/*

  DirectAccess.ino
  
  Demonstrate how to interact with the internal buffer directly
  Warning: This example will not work with all graphics controller.
  It will work with SSD13xx, UC1xxx, ST7656 controller
  It will definitly not work with ST7920, T6962, RA8835.

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

/*
	This demo can be displayed normally on OLED with a resolution of 128x64, 
	while it will be displayed incomplete in the 128x32 OLED.	
*/

#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
#include <Wire.h>

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected


/*
	Please use the corresponding instantiation-function when using the display 
	with different resolutions and communication ways.
*/
//  M0/ESP32/ESP8266/mega2560/Uno/Leonardo
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   



// End of constructor list

uint8_t background_images[8][32];

void setup(void) {
  uint8_t i;
  /* U8g2 Project: SSD1306 Test Board */
  //pinMode(10, OUTPUT);
  //pinMode(9, OUTPUT);
  //digitalWrite(10, 0);
  //digitalWrite(9, 0);		

  /* U8g2 Project: T6963 Test Board */
  //pinMode(18, OUTPUT);
  //digitalWrite(18, 1);


  u8g2.begin();
  u8g2.setFlipMode(0);

  u8g2.setFont(u8g2_font_chroma48medium8_8r);
  u8g2.setFontPosBottom();
  
  // create eight images for a background animation
  for( i = 0; i < 8; i++ ) {
    // setup buffer by starting with the first page
    u8g2.firstPage();
    // render some graphics into the buffer
    u8g2.drawStr(0-i,8,"U8g2U");
    // store four tiles from the start of the buffer content in some local memory
    // one tile has 8x8 pixel: 8 bytes, four tiles are 32 bytes.
    // note: the actual memory pattern depends on the target display
    memcpy(background_images[i], u8g2.getBufferPtr(), 32);    
  }

  // revert back to basline pos
  u8g2.setFontPosBaseline();

}

void drawBackground(uint8_t image_idx)
{
  u8g2_uint_t src_idx, dest_idx, total;
  uint8_t *buf;
  
  total = u8g2.getBufferTileWidth();
  total *= 8;	// calculate width in pixel
  
  // get the pointer to the buffer. We will write the image to that buffer
  buf = u8g2.getBufferPtr();
  
  // fill the buffer with the background image
  // repeat the image until the complete width is filled
  src_idx = 0;
  for( dest_idx = 0; dest_idx < total; dest_idx++ )
  {
    buf[dest_idx] = background_images[image_idx][src_idx];
    src_idx++;
    if ( src_idx >= 32 )
      src_idx = 0;
  }
}

void drawLogo(void)
{
    u8g2.setFontMode(1);	// Transparent

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30,"g");
    u8g2.drawStr(67,30,"\xb2");
    
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
    
}

uint8_t image_idx = 0;
uint8_t dir = 0;

void loop(void) {

  // buffer clear is not required any more 
  // this increase performance a little bit
  u8g2.setAutoPageClear(0);	

  // draw the U8g2 logo with a custom background
  u8g2.firstPage();
  do {
    drawBackground(image_idx);
    drawLogo();
  } while ( u8g2.nextPage() );
  
  // move the background around
  if ( dir == 0 ){
    image_idx++;
    if ( image_idx == 7 ) {
      dir = 1;
    }
  } else {
    image_idx--;
    if ( image_idx == 0 ) {
      dir = 0;
    }
  }    
  
  delay(10);
}

