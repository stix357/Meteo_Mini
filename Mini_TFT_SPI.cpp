
#include "Mini_TFT_SPI.h"

Mini_TFT_SPI::Mini_TFT_SPI(uint8_t CS, uint8_t DC) {
  _cs = CS;
  _dc = DC;
};
//----------
void Mini_TFT_SPI::Init() {
  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  digitalWrite(_dc, HIGH);  // Data
  SPI.begin();
  WrCommand(0x01);  // Software reset
  delay(250);     // delay 250 ms
  WrCommand(0x36);
  WrData(rotate << 5 | bgr << 3);  // Set orientation and rgb/bgr
  WrCommand(0x3A);
  WrData(0x55);              // Set color mode - 16-bit color
  WrCommand(0x20 + invert);  // Invert
  WrCommand(0x11);           // Out of sleep mode
  delay(150);
};
//--------
void Mini_TFT_SPI::Rotate(byte r) {
  WrCommand(0x36);
  WrData(r << 5 | bgr << 3);  // Set orientation and rgb/bgr
};
//--------
void Mini_TFT_SPI::DisplayOn() {
  WrCommand(0x29);  // Display on
  delay(150);
};
//---------
void Mini_TFT_SPI::DisplayOff() {
  WrCommand(0x28);  // Display OFF
  delay(150);
};
//-----------
void Mini_TFT_SPI::DisplaySleepOn() {
  WrCommand(0x10);  // Display to Sleep
  delay(150);
};
//---------
void Mini_TFT_SPI::DisplaySleepOff() {
  WrCommand(0x11);  //
  delay(150);
};
//----------
void Mini_TFT_SPI::ClearDisplay() {
  WrCommand2(CASET, yoff, yoff + ysize - 1);
  WrCommand2(RASET, xoff, xoff + xsize - 1);
  WrCommand(0x3A);
  WrData(0x03);  // 12-bit colour
  WrCommand(RAMWR);
  for (int i = 0; i < xsize / 2; i++) {
    for (int j = 0; j < ysize * 3; j++) {
      WrData(0);
    }
  }
  WrCommand(0x3A);
  WrData(0x05);  // Back to 16-bit colour
};
//-----------
//unsigned int Mini_TFT_SPI::SetColour(int r, int g, int b) {
void Mini_TFT_SPI::SetColor(int r, int g, int b) {
  fore=(r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3;
};
//------------
void Mini_TFT_SPI::MoveTo(int x, int y) {
  xpos = x;
  ypos = y;
};
//-----------
void Mini_TFT_SPI::PlotPoint(int x, int y) {
  WrCommand2(CASET, yoff + y, yoff + y);
  WrCommand2(RASET, xoff + x, xoff + x);
  WrCommand(RAMWR);
  WrData(fore >> 8);
  WrData(fore & 0xff);
};
//-----------------
void Mini_TFT_SPI::DrawTo(int x, int y) {
  int sx, sy, e2, err;
  int dx = abs(x - xpos);
  int dy = abs(y - ypos);
  if (xpos < x) sx = 1;
  else sx = -1;
  if (ypos < y) sy = 1;
  else sy = -1;
  err = dx - dy;
  for (;;) {
    PlotPoint(xpos, ypos);
    if (xpos == x && ypos == y) return;
    e2 = err << 1;
    if (e2 > -dy) {
      err = err - dy;
      xpos = xpos + sx;
    }
    if (e2 < dx) {
      err = err + dx;
      ypos = ypos + sy;
    }
  }
};
//--------------
void Mini_TFT_SPI::FillRect(int w, int h) {
  WrCommand2(CASET, ypos + yoff, ypos + yoff + h - 1);
  WrCommand2(RASET, xpos + xoff, xpos + xoff + w - 1);
  WrCommand(RAMWR);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      WrData(fore >> 8);
      WrData(fore & 0xff);
    }
  }
};
//-----------------
// void Mini_TFT_SPI::DrawByteIcon7x7(int8_t _numIcon) {
//   int colour;
//   int bits = 0;
//   WrCommand2(CASET, yoff + ypos, yoff + ypos + 8 * scale - 1);
//   WrCommand2(RASET, xoff + xpos, xoff + xpos + 8 * scale - 1);
//   WrCommand(RAMWR);
//   for (int xx = 0; xx < 7; xx++) {
//     bits = pgm_read_byte(&icons_7x7[_numIcon][xx]);
//     for (int xr = 0; xr < scale; xr++) {
//       for (int yy = 0; yy < 8; yy++) {
//         if (bits >> (7 - yy) & 1) {
//           colour = fore;
//         } else {
//           colour = back;
//         }
//         for (int yr = 0; yr < scale; yr++) {
//           WrData(colour >> 8);
//           WrData(colour & 0xFF);
//         }
//       }
//     }
//   }
//   //xpos = xpos + 8 * scale;
// };

void Mini_TFT_SPI::DrawByteIcon7x7_meteo(int8_t _numIcon) {
  int colour;
  int bits = 0;
  WrCommand2(CASET, yoff + ypos, yoff + ypos + 8 * scale - 1);
  WrCommand2(RASET, xoff + xpos, xoff + xpos + 8 * scale - 1);
  WrCommand(RAMWR);
  for (int xx = 0; xx < 7; xx++) {
    bits = pgm_read_byte(&icons_7x7_meteo[_numIcon][xx]);
    for (int xr = 0; xr < scale; xr++) {
      for (int yy = 0; yy < 8; yy++) {
        if (bits >> (7 - yy) & 1) {
          colour = fore;
        } else {
          colour = back;
        }
        for (int yr = 0; yr < scale; yr++) {
          WrData(colour >> 8);
          WrData(colour & 0xFF);
        }
      }
    }
  }
  //xpos = xpos + 8 * scale;
};
//--------------------
bool Mini_TFT_SPI::PrintCharRus(char _char) {
  int colour;
  int bits = 0;
  if ((byte)_char == 208 || (byte)_char == 209) {
    lastChar = (byte)_char;
    return true;
  }
  WrCommand2(CASET, yoff + ypos, yoff + ypos + 8 * scale - 1);
  WrCommand2(RASET, xoff + xpos, xoff + xpos + 6 * scale - 1);
  WrCommand(RAMWR);
  for (int xx = 0; xx < 6; xx++) {
    switch ((byte)_char) {
      case 0 ... 127:
        {
          bits = pgm_read_byte(&CharMapRus[(byte)_char - 32][xx]);  // для английских букв и цифр
          break;
        }
      case 128 ... 143:
        {
          if (lastChar == 208 && byte(_char) == 129) {
            bits = pgm_read_byte(&CharMapRus[101][xx]);  // Е вместо Ё
          } else {
            bits = pgm_read_byte(&CharMapRus[(byte)_char + 16][xx]);  // р-я
          };
          break;
        }
      case 144 ... 175:
        {
          if (lastChar == 209 && byte(_char) == 145) {
            bits = pgm_read_byte(&CharMapRus[160][xx]);  // ё кастомная
          } else {
            bits = pgm_read_byte(&CharMapRus[(byte)_char - 48][xx]);  // А - Я
          };
          break;
        }
      case 176 ... 191:
        {
          bits = pgm_read_byte(&CharMapRus[(byte)_char - 48][xx]);  // а-п
          break;
        }
      default:
        {
          bits = pgm_read_byte(&CharMapRus[95][xx]);
          break;
        }
    }
    for (int xr = 0; xr < scale; xr++) {
      for (int yy = 0; yy < 8; yy++) {
        if (bits >> (7 - yy) & 1) colour = fore;
        else colour = back;
        for (int yr = 0; yr < scale; yr++) {
          WrData(colour >> 8);
          WrData(colour & 0xFF);
        }
      }
    };
  }
  xpos = xpos + 6 * scale;
  return true;
};
//-------------------
size_t Mini_TFT_SPI::PrintTFTRus(const __FlashStringHelper *ifsh) {
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  size_t n = 0;
  unsigned char c;
  while (1) {
    c = pgm_read_byte(p++);
    if (c == 0) {
      break;
    }
    // if (c==208 || c==209){
    //   lastChar=c;
    //   continue;
    // }
    if (PrintCharRus(c)) {
      n++;
    } else {
      break;
    }
  }
  return n;
};
//--------------
void Mini_TFT_SPI::PrintTFTRus(const char str[]) {
  int i = 0;
  lastChar = 0;
  while (1) {
    if (str[i] == 0) return;
    PrintCharRus(str[i]);
    i++;
  }
};
//---------------
void Mini_TFT_SPI::PlotInt(int n) {
  bool lead = false;
  for (int d = 10000; d > 0; d = d / 10) {
    char j = (n / d) % 10;
    if (j != 0 || lead || d == 1) {
      PrintCharRus(j + '0');
      lead = true;
    }
  }
};
//----------------
void Mini_TFT_SPI::WrData(uint8_t d) {
  digitalWrite(_cs, LOW);
  SPI.transfer(d);
  digitalWrite(_cs, HIGH);
};
//-------------
void Mini_TFT_SPI::WrCommand(uint8_t c) {
  digitalWrite(_dc, LOW);
  WrData(c);
  digitalWrite(_dc, HIGH);
};
//---------------
void Mini_TFT_SPI::WrCommand2(uint8_t c, uint16_t d1, uint16_t d2) {
  digitalWrite(_dc, LOW);
  WrData(c);
  digitalWrite(_dc, HIGH);
  WrData(d1 >> 8);  //старший
  WrData(d1);       //младший
  WrData(d2 >> 8);
  WrData(d2);
};