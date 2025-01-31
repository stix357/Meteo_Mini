/* Compact TFT Graphics Library v4 - see http://www.technoblogy.com/show?2LMJ

   David Johnson-Davies - www.technoblogy.com - 9th May 2022
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

#ifndef Mini_TFT_SPI_h
#define Mini_TFT_SPI_h

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
#include "CharMapRus.h"
#pragma once
//#include "icons_7x7.h"
#include "icons_7x7_meteo.h"
// TFT colour display **********************************************
// Display parameters - uncomment the line for the one you want to use

// Adafruit 1.44" 128x128 display
// int const xsize = 128, ysize = 128, xoff = 2, yoff = 1, invert = 0, rotate = 3, bgr = 1;

// AliExpress 1.44" 128x128 display
// int const xsize = 128, ysize = 128, xoff = 2, yoff = 1, invert = 0, rotate = 3, bgr = 1;

// Adafruit 0.96" 160x80 display
//int const xsize = 160, ysize = 80, xoff = 0, yoff = 24, invert = 1, rotate = 6, bgr = 1;

// AliExpress 0.96" 160x80 display
//int const xsize = 160, ysize = 80, xoff = 1, yoff = 26, invert = 1, rotate = 6, bgr = 1;
int const xsize = 160, ysize = 128, X0 = 0, Y0 = 0, invert = 0, rotate = 6, bgr = 0;
// Adafruit 1.8" 160x128 display
// int const xsize = 160, ysize = 128, xoff = 0, yoff = 0, invert = 0, rotate = 0, bgr = 1;

// AliExpress 1.8" 160x128 display (red PCB)
// int const xsize = 160, ysize = 128, xoff = 0, yoff = 0, invert = 0, rotate = 0, bgr = 1;

// AliExpress 1.8" 160x128 display (blue PCB)
// int const xsize = 160, ysize = 128, xoff = 0, yoff = 0, invert = 0, rotate = 6, bgr = 0;

// Adafruit 1.14" 240x135 display
// int const xsize = 240, ysize = 135, xoff = 40, yoff = 53, invert = 1, rotate = 6, bgr = 0;

// AliExpress 1.14" 240x135 display
// int const xsize = 240, ysize = 135, xoff = 40, yoff = 52, invert = 1, rotate = 0, bgr = 0;

// Adafruit 1.3" 240x240 display
// int const xsize = 240, ysize = 240, xoff = 0, yoff = 80, invert = 1, rotate = 5, bgr = 0;

// Adafruit 1.54" 240x240 display
// int const xsize = 240, ysize = 240, xoff = 0, yoff = 80, invert = 1, rotate = 5, bgr = 0;

// AliExpress 1.54" 240x240 display
//int const xsize = 240, ysize = 240, xoff = 0, yoff = 80, invert = 1, rotate = 5, bgr = 0;

// Adafruit 1.9" 320x170 display
// int const xsize = 320, ysize = 170, xoff = 0, yoff = 35, invert = 1, rotate = 0, bgr = 0;

// Adafruit 2.0" 320x240 display
//int const xsize = 320, ysize = 240, xoff = 0, yoff = 0, invert = 1, rotate = 6, bgr = 0;

// Adafruit 2.2" 320x240 display
// int const xsize = 320, ysize = 240, xoff = 0, yoff = 0, invert = 0, rotate = 4, bgr = 1;

// AliExpress 2.4" 320x240 display
// int const xsize = 320, ysize = 240, xoff = 0, yoff = 0, invert = 0, rotate = 2, bgr = 1;
//-------------------
// // some flags for initR() :(
// #define INITR_GREENTAB 0x0
// #define INITR_REDTAB   0x1
// #define INITR_BLACKTAB   0x2

// #define ST7735_TFTWIDTH  128
// #define ST7735_TFTHEIGHT 160

// #define ST7735_NOP     0x00
// #define ST7735_SWRESET 0x01
// #define ST7735_RDDID   0x04
// #define ST7735_RDDST   0x09

// #define ST7735_SLPIN   0x10
// #define ST7735_SLPOUT  0x11
// #define ST7735_PTLON   0x12
// #define ST7735_NORON   0x13

// #define ST7735_INVOFF  0x20
// #define ST7735_INVON   0x21
// #define ST7735_DISPOFF 0x28
// #define ST7735_DISPON  0x29
// #define ST7735_CASET   0x2A
// #define ST7735_RASET   0x2B
// #define ST7735_RAMWR   0x2C
// #define ST7735_RAMRD   0x2E

// #define ST7735_PTLAR   0x30
// #define ST7735_COLMOD  0x3A
// #define ST7735_MADCTL  0x36

// #define ST7735_FRMCTR1 0xB1
// #define ST7735_FRMCTR2 0xB2
// #define ST7735_FRMCTR3 0xB3
// #define ST7735_INVCTR  0xB4
// #define ST7735_DISSET5 0xB6

// #define ST7735_PWCTR1  0xC0
// #define ST7735_PWCTR2  0xC1
// #define ST7735_PWCTR3  0xC2
// #define ST7735_PWCTR4  0xC3
// #define ST7735_PWCTR5  0xC4
// #define ST7735_VMCTR1  0xC5

// #define ST7735_RDID1   0xDA
// #define ST7735_RDID2   0xDB
// #define ST7735_RDID3   0xDC
// #define ST7735_RDID4   0xDD

// #define ST7735_PWCTR6  0xFC

// #define ST7735_GMCTRP1 0xE0
// #define ST7735_GMCTRN1 0xE1

// // Color definitions
// #define	ST7735_BLACK   0x0000
// #define	ST7735_BLUE    0x001F
// #define	ST7735_RED     0xF800
// #define	ST7735_GREEN   0x07E0
// #define ST7735_CYAN    0x07FF
// #define ST7735_MAGENTA 0xF81F
// #define ST7735_YELLOW  0xFFE0  
// #define ST7735_WHITE   0xFFFF
//------------
int const CASET = 0x2A;  // Define column address
int const RASET = 0x2B;  // Define row address
int const RAMWR = 0x2C;  // Write to display RAM
//--------------------
class Mini_TFT_SPI {
public:
  // Arduino pin numbers. Change these for your display connections
  //int const cs = 10;  // TFT display SPI chip select pin
  //int const dc = 9;   // TFT display data/command select pin
  Mini_TFT_SPI(uint8_t CS = 10, uint8_t DC = 9);

  // // Globals - current plot position and colours
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
  void SetColor(int r, int g, int b);
  // Move current plot position to x,y
  void MoveTo(int x, int y);
  // Plot point at x,y
  void PlotPoint(int x, int y);
  // Draw a line to x,y
  void DrawTo(int x, int y);
  void FillRect(int w, int h);
  //  DRAW an ByteIcon7x7 with bottom left corner at x,y
  //void DrawByteIcon7x7(int8_t _numIcon);
  void DrawByteIcon7x7_meteo(int8_t _numIcon);  
  //  Plot an ASCII character with bottom left corner at x,y
  bool PrintCharRus(char _char);
  // Plot text starting at the current plot position
  size_t PrintTFTRus(const __FlashStringHelper *ifsh);
  void PrintTFTRus(const char str[]);
  //--------------
  void PlotInt(int n);
  // // Plot an ASCII character with bottom left corner at x,y
  // void PlotChar(char _char) {
  //   int colour;
  //   int bits = 0;
  //   byte codeChar = (byte)_char;
  //   Command2(CASET, yoff + ypos, yoff + ypos + 8 * scale - 1);
  //   Command2(RASET, xoff + xpos, xoff + xpos + 6 * scale - 1);
  //   Command(RAMWR);
  //   for (int xx = 0; xx < 6; xx++) {
  //     bits = pgm_read_byte(&CharMap[_char - 32][xx]);
  //     for (int xr = 0; xr < scale; xr++) {
  //       for (int yy = 0; yy < 8; yy++) {
  //         if (bits >> (7 - yy) & 1) colour = fore;
  //         else colour = back;
  //         for (int yr = 0; yr < scale; yr++) {
  //           Data(colour >> 8);
  //           Data(colour & 0xFF);
  //         }
  //       }
  //     }
  //   }
  //   xpos = xpos + 6 * scale;
  // }
  // // Plot text starting at the current plot position
  // void PlotText(PGM_P p) {
  //   while (1) {
  //     char c = pgm_read_byte(p++);
  //     if (c == 0) return;
  //     PlotChar(c);
  //   }
  // }

  // //--------------------------
private:
  //---------------------------
  byte lastChar = 0;
  uint8_t _cs = 10;
  uint8_t _dc = 9;
  //------------------
  //Send a byte to the display
  void WrData(uint8_t d);
  // Send a command to the display
  void WrCommand(uint8_t c);
  // Send a command followed by two data words
  void WrCommand2(uint8_t c, uint16_t d1, uint16_t d2);
};
#endif  //Mini_TFT_SPI_H