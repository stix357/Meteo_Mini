
#include "TFT_SPI_ST77XX.h"
//#include "fonts/UTFTFonts.h"

struct _current_font {
  uint8_t *font;
  uint8_t x_size;
  uint8_t y_size;
  uint8_t offset;
  uint8_t numchars;
};

_current_font cfont;

TFT_SPI_ST77XX::TFT_SPI_ST77XX(uint8_t CS, uint8_t DC, uint8_t RS) {
  _cs = CS;
  _dc = DC;
  _rst = RS;
};
//----------
void TFT_SPI_ST77XX::Init() {
  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_dc, HIGH);  // Data
  //HardReset();
  //delay(250);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);  //// 8 MHz (full! speed!)
  WrCommand(ST77XX_SWRESET);            // Software reset 0x01
  delay(250);                           // delay 250 ms
  WrCommand(ST77XX_MADCTL);             //0x36
  WrData(rotate << 5 | bgr << 3);       // Set orientation and rgb/bgr
  //WrData(MY << 7 | MX << 6 | rotate << 5 | bgr << 3);
  WrCommand(ST77XX_COLMOD);  //0x3A
  //WrData(0x55);
  WrData(ST77XX_COLMOD_16);           // Set color mode - 16-bit color
  WrCommand(ST77XX_INVOFF + invert);  // Invert
  WrCommand(ST77XX_NORON);            //// Partial Off (Normal)
  WrCommand(ST77XX_SLPOUT);           // Out of sleep mode 0x11
  delay(150);
};
//--------
void TFT_SPI_ST77XX::Rotate(byte r) {
  _MV = r & 2;
  //WrCommand(0x36);
  WrCommand(ST77XX_MADCTL);
  WrData(r << 5 | bgr << 3);  // Set orientation and rgb/bgr
};
//--------
void TFT_SPI_ST77XX::DisplayOn() {
  WrCommand(ST77XX_DISPON);  // Display on 0x29
  delay(150);
};
//---------
void TFT_SPI_ST77XX::DisplayOff() {
  WrCommand(ST77XX_DISPOFF);  // Display OFF 0x28
  delay(150);
};
//-----------
void TFT_SPI_ST77XX::DisplaySleepOn() {
  WrCommand(ST77XX_SLPIN);  // Display to Sleep 0x10
  delay(150);
};
//---------
void TFT_SPI_ST77XX::DisplaySleepOff() {
  WrCommand(ST77XX_SLPOUT);  //0x11
  delay(150);
};
//----------
void TFT_SPI_ST77XX::ClearDisplay() {
  WrCommand2(ST77XX_RASET, X0, X0 + xsize - 1);
  WrCommand2(ST77XX_CASET, Y0, Y0 + ysize - 1);
  WrCommand(ST77XX_COLMOD);  //0x3A
  WrData(ST77XX_COLMOD_12);  // 12-bit colour 0x03
  WrCommand(ST77XX_RAMWR);
  // for (int i = 0; i < xsize / 2; i++) {
  //   for (int j = 0; j < ysize * 3; j++) {
  //     WrData(0);
  //   }
  // }
  if (_MV == 0) {
    for (int i = 0; i < xsize / 2; i++) {
      for (int j = 0; j < ysize * 3; j++) {
        WrData(0);
      };
    };
  } else {
    for (int i = 0; i < ysize / 2; i++) {
      for (int j = 0; j < xsize * 3; j++) {
        WrData(0);
      };
    };
  };
  WrCommand(ST77XX_COLMOD);
  WrData(ST77XX_COLMOD_16);  // Back to 16-bit colour 0x05
};
//-----------
//unsigned int TFT_SPI_ST77XX::SetColour(int r, int g, int b) {
void TFT_SPI_ST77XX::SetColor(int r, int g, int b) {
  fore = (r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3;
};
//------------
void TFT_SPI_ST77XX::MoveTo(int x, int y) {
  xpos = x;
  ypos = y;
};
//-----------
void TFT_SPI_ST77XX::PlotPoint(int x, int y) {
  WrCommand2(ST77XX_RASET, X0 + x, X0 + x);
  WrCommand2(ST77XX_CASET, Y0 + y, Y0 + y);
  WrCommand(ST77XX_RAMWR);
  WrData(fore >> 8);
  WrData(fore & 0xff);
};
//-----------------
void TFT_SPI_ST77XX::DrawTo(int x, int y) {
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
void TFT_SPI_ST77XX::FillRect(int w, int h) {
  WrCommand2(ST77XX_RASET, xpos + X0, xpos + X0 + w - 1);
  WrCommand2(ST77XX_CASET, ypos + Y0, ypos + Y0 + h - 1);
  WrCommand(ST77XX_RAMWR);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      WrData(fore >> 8);
      WrData(fore & 0xff);
    }
  }
};
//-----------------
void TFT_SPI_ST77XX::DrawByteIcon7x7(int8_t _numIcon) {
  int colour;
  int bits = 0;
  WrCommand2(ST77XX_RASET, X0 + xpos, X0 + xpos + 8 * scale - 1);
  WrCommand2(ST77XX_CASET, Y0 + ypos, Y0 + ypos + 8 * scale - 1);
  WrCommand(ST77XX_RAMWR);
  for (int xx = 0; xx < 7; xx++) {
    bits = pgm_read_byte(&icons_7x7[_numIcon][xx]);
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
//-----------------
//obj.drawBytes_P(GFX_icons::email, 8);
void TFT_SPI_ST77XX::DrawByteIcon8x8(const uint8_t *bytes, int amount) {
  int colour;
  int bits = 0;
  WrCommand2(ST77XX_RASET, X0 + xpos, X0 + xpos + 8 * scale - 1);
  WrCommand2(ST77XX_CASET, Y0 + ypos, Y0 + ypos + 8 * scale - 1);
  WrCommand(ST77XX_RAMWR);
  for (int xx = 0; xx < amount; xx++) {
    bits = pgm_read_byte(&bytes[xx]);
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
bool TFT_SPI_ST77XX::PrintCharRus(char _char) {
  int colour;
  int bits = 0;
  if ((byte)_char == 208 || (byte)_char == 209) {
    lastChar = (byte)_char;
    return true;
  };
  WrCommand2(ST77XX_RASET, X0 + xpos, X0 + xpos + 6 * scale - 1);
  WrCommand2(ST77XX_CASET, Y0 + ypos, Y0 + ypos + 8 * scale - 1);
  WrCommand(ST77XX_RAMWR);
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
        };
    };
    for (int xr = 0; xr < scale; xr++) {
      for (int yy = 0; yy < 8; yy++) {
        if (bits >> (7 - yy) & 1) {
          colour = fore;
        } else {
          colour = back;
        };
        for (int yr = 0; yr < scale; yr++) {
          WrData(colour >> 8);
          WrData(colour & 0xFF);
        }
      };
#ifdef SERIAL_DEGUG
      if (bits < 0x10) {
        Serial.print("bits=0x0");
      } else {
        Serial.print("bits=0x");
      }
      Serial.println(bits, HEX);
#endif
    };
  };
  xpos = xpos + 6 * scale;
  return true;
};
//-------------------
size_t TFT_SPI_ST77XX::PrintTFTRus(const __FlashStringHelper *ifsh) {
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
void TFT_SPI_ST77XX::PrintTFTRus(const char str[]) {
  int i = 0;
  lastChar = 0;
  while (1) {
    if (str[i] == 0) return;
    PrintCharRus(str[i]);
    i++;
  }
};
//---------------
void TFT_SPI_ST77XX::PlotInt(int n) {
  bool lead = false;
  for (int d = 10000; d > 0; d = d / 10) {
    char j = (n / d) % 10;
    if (j != 0 || lead || d == 1) {
      PrintCharRus(j + '0');
      lead = true;
    }
  }
};
//-----------------
void TFT_SPI_ST77XX::HardReset() {
  digitalWrite(_rst, LOW);
  delay(250);
  digitalWrite(_rst, HIGH);
};

void TFT_SPI_ST77XX::SoftReset() {
  WrCommand(ST77XX_SWRESET);  // Software reset 0x01
  delay(250);                 // delay 250 ms
};

void TFT_SPI_ST77XX::FillDisplay_XY(int16_t _color) {
  uint32_t pixels = xsize * ysize;
  WrCommand2(ST77XX_RASET, X0, X0 + xsize - 1);
  WrCommand2(ST77XX_CASET, Y0, Y0 + ysize - 1);
  WrCommand(ST77XX_RAMWR);
  for (uint32_t n = 0; n < pixels; n++) {
    //WrData(_color);
    WrData(_color >> 8);
    WrData(_color & 0xff);
  };
};

void TFT_SPI_ST77XX::FillDisplay_YX(int16_t _color) {
  uint32_t pixels = xsize * ysize;
  WrCommand2(ST77XX_CASET, 0, xsize+32 - 1);
  WrCommand2(ST77XX_RASET, 0, ysize - 1);
  WrCommand(ST77XX_RAMWR);
  for (uint32_t n = 0; n < pixels; n++) {
    //WrData(_color);
    WrData(_color >> 8);
    WrData(_color & 0xff);
  };
};

//----------------
void TFT_SPI_ST77XX::WrData(uint8_t d) {
  digitalWrite(_cs, LOW);
  SPI.transfer(d);
  digitalWrite(_cs, HIGH);
};
//--------------- команда без параметров
void TFT_SPI_ST77XX::WrCommand(uint8_t c) {
  digitalWrite(_dc, LOW);
  WrData(c);
  digitalWrite(_dc, HIGH);
};
//--------------- команда с параметрами d1 d2
void TFT_SPI_ST77XX::WrCommand2(uint8_t c, uint16_t p1, uint16_t p2) {
  digitalWrite(_dc, LOW);
  WrData(c);
  digitalWrite(_dc, HIGH);
  WrData(p1 >> 8);  //старший
  WrData(p1);       //младший
  WrData(p2 >> 8);
  WrData(p2);
};
//--------------
void TFT_SPI_ST77XX::SetFont(const uint8_t *_font) {
  cfont.font = _font;
  cfont.x_size = fontbyte(0);
  cfont.y_size = fontbyte(1);
  cfont.offset = fontbyte(2);
  cfont.numchars = fontbyte(3);
};

uint8_t TFT_SPI_ST77XX::PrintDig7s(byte _c) {
  uint16_t temp;
  byte ch;
  int colour;
//Rotate(rotate-1);
//----------------------
#ifdef SERIAL_DEGUG
  if (_c < 0x10) {
    Serial.print("_c=0x0"); 
  } else {
    Serial.print("_c=0x");
  }
  Serial.println(_c, HEX);
  Serial.print("cfont.y_size=");
  Serial.println(cfont.y_size, HEX); 
  Serial.print("cfont.x_size=");
  Serial.println(cfont.x_size, HEX); 
#endif
  if (_c < 0x30 || _c > 0x39) {
    return 0;
  };
  WrCommand2(ST77XX_RASET, xpos + X0, xpos + X0 + cfont.y_size - 1);
  WrCommand2(ST77XX_CASET, ypos + Y0, ypos + Y0 + cfont.x_size - 1);
  WrCommand(ST77XX_RAMWR);
  temp = ((_c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
  for (int j = 0; j < ((cfont.x_size / 8) * cfont.y_size); j++) {
    ch = pgm_read_byte(&cfont.font[temp]);
#ifdef SERIAL_DEGUG
    Serial.print("temp=");
    Serial.println(temp);
    if (ch < 0x10) {
      Serial.print("ch=0x0");
    } else {
      Serial.print("ch=0x");
    }
    Serial.println(ch, HEX);
#endif
    for (int i = 0; i < 8; i++) {
      if ((ch & (1 << (7 - i))) != 0) {
        //setPixel((fch << 8) | fcl);
        colour = fore;
      } else {
        //setPixel((bch << 8) | bcl);
        colour = back;
      }
      WrData(colour >> 8);
      WrData(colour & 0xFF);
    }
    temp++;
  };
  // #ifdef SERIAL_DEGUG
  //   Serial.print("_c=");
  //   Serial.println(_c);
  //   Serial.print("_ch=");
  //   Serial.println(ch);
  //   Serial.print("cfont.x_size =");
  //   Serial.println(cfont.x_size);
  //   Serial.print("cfont.y_size=");
  //   Serial.println(cfont.y_size);
  // #endif
  //Rotate(rotate);
  //xpos = xpos + cfont.x_size;
  //ypos = ypos + cfont.x_size;
  return cfont.x_size;
};


void TFT_SPI_ST77XX::Print_DigArr7s(const char str[]) {
  int i = 0;
  int y = 0;
  Rotate(6);
  while (1) {
    if (str[i] == 0) return;
    y = PrintDig7s(str[i]);
    ypos = ypos + y;
    i++;
  };
  Rotate(rotate);
};

void TFT_SPI_ST77XX::DrawByteIcon7x7_meteo(int8_t _numIcon) {
  int colour;
  int bits = 0;
  WrCommand2(ST77XX_RASET, X0 + xpos, X0 + xpos + 8 * scale - 1);
  WrCommand2(ST77XX_CASET, Y0 + ypos, Y0 + ypos + 8 * scale - 1);
  WrCommand(ST77XX_RAMWR);
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
  xpos = xpos + 8 * scale;
};

void TFT_SPI_ST77XX::DrawLine(int _x0, int _y0, int _xe, int _ye) {
  xpos = _x0;
  ypos = _y0;
  for (;;) {
    PlotPointXY(xpos, ypos);
    if (xpos == _xe && ypos == _ye) return;
    if (_xe > _x0) {
      xpos = xpos + 1;
      ypos = ypos + 1;
    } else {
      xpos = xpos - 1;
    };
    if (_ye > _y0) {
      ypos = ypos + 1;
    } else {
      ypos = ypos - 1;
    };
  };
};

//-----------
void TFT_SPI_ST77XX::PlotPointXY(int x, int y) {
  WrCommand2(ST77XX_RASET, x, x);
  WrCommand2(ST77XX_CASET, y, y);
  WrCommand(ST77XX_RAMWR);
  WrData(fore >> 8);
  WrData(fore & 0xff);
};
