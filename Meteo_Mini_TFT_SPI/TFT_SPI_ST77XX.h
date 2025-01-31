/* Compact TFT Graphics Library v4 - see http://www.technoblogy.com/show?2LMJ

   David Johnson-Davies - www.technoblogy.com - 9th May 2022
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

#ifndef TFT_SPI_ST77XX_H
#define TFT_SPI_ST77XX_H

// #if ARDUINO >= 100
//  #include "Arduino.h"
//  #include "Print.h"
// #else
//  #include "WProgram.h"
// #endif
#pragma once
#include <Arduino.h>
#pragma once
#include <SPI.h>
#pragma once
#include "fonts/CharMapRus.h"
#pragma once
#include "fonts/icons_7x7.h"
#pragma once
#include "fonts/icons_7x7_meteo.h"
// TFT colour display **********************************************
// Display parameters - uncomment the line for the one you want to use

// Adafruit 1.44" 128x128 display
// int const xsize = 128, ysize = 128, X0 2, yoff = 1, invert = 0, rotate = 3, bgr = 1;

// AliExpress 1.44" 128x128 display
// int const xsize = 128, ysize = 128, X0 2, yoff = 1, invert = 0, rotate = 3, bgr = 1;

// Adafruit 0.96" 160x80 display
//int const xsize = 160, ysize = 80, X0 0, yoff = 24, invert = 1, rotate = 6, bgr = 1;

// AliExpress 0.96" 160x80 display
//int const xsize = 160, ysize = 80, X0 1, yoff = 26, invert = 1, rotate = 6, bgr = 1;

// Adafruit 1.8" 160x128 display
// MADCTL
//               D7 D6 D5 D4 D3  D2 D1 D0
// Parameter 1   MY MX MV ML RGB MH -  -


// MY Row Address Order 
// MX Column Address Order
// MV Row/Column Exchange
// These 3bits controls MCU to memory write/read direction
// В режиме вертикальной адресации (MV=1) значение Y-адреса увеличивается после каждого байта, после последнего Y-адреса 
// (Y=YE) значение Y переходит в значение YS, а значение X увеличивается для адреса следующего столбца. 
// В режиме горизонтальной адресации 
// (MV=0), X-адрес увеличивается после каждого байта, после последнего X-адреса (X=XE) X преобразуется в XS и 
// Y увеличивается для адресации следующей строки. 

// AliExpress 1.8" 160x128 display (red PCB)
// int const xsize = 160, ysize = 128, X0 0, yoff = 0, invert = 0, rotate = 0, bgr = 1;

// AliExpress 1.8" 160x128 display (blue PCB)
// int const xsize = 160, ysize = 128, X0 0, yoff = 0, invert = 0, rotate = 6, bgr = 0;


// AliExpress 2.0" 320x240 display
//int const xsize = 320, ysize = 240, X0 = 0, Y0 = 0, invert = 1, rotate = 0, bgr = 0;
//int const xsize = 320, ysize = 240, X0 = 0, Y0 = 0, invert = 1, rotate = 0, bgr = 0;
// AliExpress 1.8" 160x128 display (blue PCB)
// extern uint8_t SmallFont[];                    // подключаем маленький шрифт
// extern uint8_t BigFont[];                      // подключаем большой шрифт
// extern uint8_t SevenSegNumFont[];              // подключаем шрифт имитирующий семисегментный индикатор

int const xsize = 128, ysize = 160, X0 = 0, Y0 = 32, invert = 0, rotate = 3, bgr = 0;
//int const xsize = 160, ysize = 80, X0 = 1, Y0 = 26, invert = 1, rotate = 6, bgr = 1;

#define ST77XX_COLMOD_12 0x03 // 12-bit/Pixel (RGB 4-4-4-bit Input), 4K-Colors, 3AH=“03h”
#define ST77XX_COLMOD_16 0x05 // 16-bit/Pixel (RGB 5-6-5-bit Input), 65K-Colors, 3AH=“05h”
#define ST77XX_NOP 0x00
#define ST77XX_SWRESET 0x01 
#define ST77XX_RDDID 0x04
#define ST77XX_RDDST 0x09

#define ST77XX_SLPIN 0x10 // Sleep In & Booster Off 
#define ST77XX_SLPOUT 0x11 // Sleep Out & Booster On
#define ST77XX_PTLON 0x12 // Partial Mode On
#define ST77XX_NORON 0x13 // Partial Off (Normal)

#define ST77XX_INVOFF 0x20 //  Display Inversion Off (Normal)
#define ST77XX_INVON 0x21
#define ST77XX_DISPOFF 0x28 // Display Off 
#define ST77XX_DISPON 0x29
#define ST77XX_CASET 0x2A // Column Address Set 
#define ST77XX_RASET 0x2B // Row Address Set
#define ST77XX_RAMWR 0x2C // Memory Write 
#define ST77XX_RAMRD 0x2E // Memory Read 
 //#define ST7735_RGBSET  0x2D // LUT for 4k,65k,262k Color display

#define ST77XX_PTLAR 0x30
#define ST77XX_TEOFF 0x34
#define ST77XX_TEON 0x35
#define ST77XX_MADCTL 0x36 //Memory Data Access Control
#define ST77XX_COLMOD 0x3A

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1 0xDA
#define ST77XX_RDID2 0xDB
#define ST77XX_RDID3 0xDC
#define ST77XX_RDID4 0xDD

// Some ready-made 16-bit ('565') color settings:
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00
//---------------

// This is a structure to conveniently hold infomation on the fonts
// Stores font character image address pointer, width table and height
//#define SERIAL_DEGUG

#define fontbyte(x) cfont.font[x];
//--------------------
class TFT_SPI_ST77XX {
public:
  // Arduino pin numbers. Change these for your display connections
  //int const cs = 10;  // TFT display SPI chip select pin
  //int const dc = 9;   // TFT display data/command select pin
  //int _rs = 8; //for hard reset (RST pin on TFT)
  TFT_SPI_ST77XX(uint8_t CS = 10, uint8_t DC = 9, uint8_t RS = 8);

  // Globals - current plot position and colours
  int xpos, ypos;
  int fore = 0xFFFF;  // White
  int back = 0;       // Black
  int scale = 1;      // Text scale
  //--------------------------------------
  void Init();
  void Rotate(byte r);
  void DisplayOn();
  void DisplayOff();
  void DisplaySleepOn();
  void DisplaySleepOff();
  void ClearDisplay();
  void FillDisplay_XY(int16_t _color);
  void FillDisplay_YX(int16_t _color);
  void SetColor(int r, int g, int b);
  // Move current plot position to x,y
  void MoveTo(int x, int y);
  // Plot point at x,y
  void PlotPoint(int x, int y);
  // Draw a line to x,y
  void DrawTo(int x, int y);
  void FillRect(int w, int h);
  //  DRAW an ByteIcon7x7 with bottom left corner at x,y
  void DrawByteIcon7x7(int8_t _numIcon);
  void DrawByteIcon8x8(const uint8_t *bytes, int amount);
  //  Plot an ASCII character with bottom left corner at x,y
  bool PrintCharRus(char _char);
  // Plot text starting at the current plot position
  size_t PrintTFTRus(const __FlashStringHelper *ifsh);
  void PrintTFTRus(const char str[]);
  //------------------
    void SetFont(const uint8_t *font);
  uint8_t PrintDig7s(byte _ch);
  void Print_DigArr7s(const char str[]);
  //--------------
  void PlotInt(int n);
  void HardReset();
  void SoftReset();
  void DrawByteIcon7x7_meteo(int8_t _numIcon);
  void PlotPointXY(int x, int y);
  void DrawLine(int _x0, int _y0, int _xe, int _ye);
  //--------------------------
private:
  //---------------------------
  byte lastChar = 0;
  uint8_t _cs = 10;
  uint8_t _dc = 9;
  uint8_t _rst = 8; //for hard reset (RST pin on TFT)
  byte _MV = 0;
  //------------------
  //Send a byte to the display
  void WrData(uint8_t d);
  // Send a command to the display
  void WrCommand(uint8_t c);
  // Send a command followed by two data words
  void WrCommand2(uint8_t c, uint16_t p1, uint16_t p2);
};
#endif  //TFT_SPI_ST77XX_H