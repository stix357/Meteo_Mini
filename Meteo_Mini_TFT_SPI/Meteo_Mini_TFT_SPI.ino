//#include "MINI_TFT_SPI.h"
#include "TFT_SPI_ST77XX.h"
//#include "fonts/icons8x8.h"
#include "fonts/UTFTFonts2.h"
#include "SHT21.h"
//#include "fonts/icons_7x7_meteo.h"
//#include "HX710B.h"
//#include <GyverHX711.h>
#include <GyverBME280.h>

#define heightChar 8
#define btnPin 5

//-----------------
#define SERIALDEBUG
//----------------

//Mini_TFT_SPI tft;
TFT_SPI_ST77XX tft;
SHT21 sht;
GyverBME280 bmp;
//HX710B press_sens;
//GyverHX711 press_sens(7, 6, HX_GAIN64_A);

float temp = 0;
float humidity = 0;
float press = 0;
char sensorPrintout[5];
uint32_t timer30s = 0;
//uint32_t timer2 = 0;
//uint8_t codeOper = 0;
void setup() {
  //-------------
#ifdef SERIALDEBUG
  Serial.begin(57600);
  //if (!bmp.begin(0x76)) Serial.println("Error!");
  //Serial.print(F("Size CharMap= "));
  // Serial.println(sizeof(CharMap));
#endif
  // setup code
  pinMode(btnPin, INPUT);
  Wire.begin();
  bmp.begin(0x76);
  delay(1000);
  tft.Init();
  //tft.Rotate(3);
  //tft.ClearDisplay();
  tft.FillDisplay_YX(0);
  tft.DisplayOn();
  //----------------------
  tft.scale = 2;
  tft.SetColor(0, 255, 255);
  tft.DrawLine(0,0,127,0);
  //tft.MoveTo(0, 0);
  //tft.DrawTo(159, 0);
  //tft.DrawTo(159, 127);
  //tft.DrawTo(0, 127);
  //tft.DrawTo(0, 0);
  //---------------------
  tft.scale = 1;
  tft.SetColor(255, 0, 0);
  tft.MoveTo(20, 90);
  tft.DrawByteIcon7x7_meteo(1);
  //tft.MoveTo(20, 90);
  //tft.DrawByteIcon7x7_meteo(1);
  tft.SetColor(0, 255, 0);
  tft.PrintTFTRus(F(" ТЕМПЕРАТУРА"));
  //---------------------------
  tft.SetColor(0, 70, 255); 
  tft.MoveTo(20, 60);
  tft.DrawByteIcon7x7_meteo(2);
  tft.PrintTFTRus(F(" ВЛАЖНОСТЬ"));
  //----------------------
  tft.SetColor(255, 255, 255);
  tft.MoveTo(20, 30);
  tft.DrawByteIcon7x7_meteo(3);
  tft.PrintTFTRus(F(" ДАВЛЕНИЕ"));
  //----------------------------
  //press.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //press_sens.begin(7, 6, 128);
  tft.SetFont(SevenSegNumFont);
  //---------------------------
  delay(5000);
  //tft.ClearDisplay();
  tft.FillDisplay_YX(0);
  tft.MoveTo(0, 0);
  tft.DisplayOff();
  //tft.scale = 3;
  timer30s = millis();
}

void loop() {
  if (digitalRead(btnPin) == 1) {
    timer30s = millis();
    #ifdef SERIALDEBUG
    Serial.println("Press btnPin=1");  // print readings
    #endif
  };
  //---------------------------
  if (millis() - timer30s < 30000) {  // таймер timer30s
  tft.scale = 2;
  tft.SetColor(0, 255, 0);
  //tft.DrawLine(0,0,127,0);
  //tft.DrawLine(127,0,127,159);
  //---------------------------------
    temp = round(sht.getTemperature())-2;
    //temp = bmp.readTemperature();
    humidity = round(sht.getHumidity()) + 16;//костыль
    //press = press_sens.mmHg() + 1120;
    //press = press_sens.get_mmHg(5);
    press = round(bmp.readPressure()*0.00750062);
    //------------------temp
    String sensorT(temp, 0);
    #ifdef SERIALDEBUG
    Serial.print("sensorPrintout="); 
    Serial.println(sensorPrintout); 
    #endif
    sensorT.toCharArray(sensorPrintout, 5);
    //tft.Rotate(0);
    //tft.scale = 3;
    //tft.SetColor(255, 165, 0);
    //tft.SetColor(255, 255, 255);
    //tft.DrawByteIcon7x7_meteo(1);
    //tft.PrintTFTRus(F("T "));
    //------------------
    tft.MoveTo(5, 7);
    tft.SetColor(255, 255, 0);
    tft.Print_DigArr7s(sensorPrintout);
    tft.scale = 3;
    tft.Rotate(3);
    tft.MoveTo(tft.xpos+105,tft.ypos+25);
    tft.PrintTFTRus(F("C"));
    tft.Rotate(rotate);
    //tft.PrintTFTRus(sensorPrintout);
    //tft.PrintTFTRus(F(" "));
    //tft.DrawByteIcon7x7(86);
    // tft.DrawByteIcon7x7_meteo(0);
    // //---------------------------------humidity
    tft.MoveTo(55,7 );
    tft.SetColor(0, 70, 255);
    // tft.DrawByteIcon7x7_meteo(2);
    String sensorH(humidity, 0);
    sensorH.toCharArray(sensorPrintout, 5);
    tft.Print_DigArr7s(sensorPrintout);
    tft.scale = 3;
    tft.Rotate(2);
    tft.MoveTo(tft.xpos+30,tft.ypos);
    tft.PrintTFTRus(F("%"));
    tft.Rotate(rotate);
    // //tft.PrintTFTRus(F("B "));
    // tft.PrintTFTRus(sensorPrintout);
    // tft.PrintTFTRus(F(" %"));
    // //-----------------------------------press
     tft.SetColor(255, 255, 255);
     tft.MoveTo(105, -25);
    // tft.DrawByteIcon7x7_meteo(3);
    String sensorP(press, 0);
    sensorP.toCharArray(sensorPrintout, 5);
    tft.Print_DigArr7s(sensorPrintout);
    tft.scale = 1;
    tft.Rotate(3);
    tft.MoveTo(tft.xpos+5,tft.ypos-60);
    tft.PrintTFTRus(F("ММ."));
    tft.MoveTo(tft.xpos-18,tft.ypos-10);
    tft.PrintTFTRus(F("РТ."));
      tft.MoveTo(tft.xpos-18,tft.ypos-10);
    tft.PrintTFTRus(F("СТ."));
    tft.Rotate(rotate);
    // //tft.PrintTFTRus(F("Д "));
    // tft.PrintTFTRus(sensorPrintout);
    // tft.PrintTFTRus(F(" мм"));
    tft.DisplayOn();
    //---------------------------------
#ifdef SERIALDEBUG
    Serial.print(" Temp: ");  // print readings
    Serial.println(temp, 2);
    // Serial.print(" Humidity: ");
    // Serial.println(humidity, 2);
    // //if (press_sens.is_ready()) {
    // if (press_sens.available()) {
    //   //Serial.println(" HX710B reading: ");
    //   Serial.print(" Pascal: ");
    //   Serial.println(press_sens.get_kpascal(5), 2);
    //   //Serial.print(" ATM: ");
    //   //Serial.println(press_sens.atm(), 2);
    //   Serial.print(" mmHg: ");
    //  // Serial.println(press_sens.mmHg(), 2);
    //   Serial.println(press_sens.get_mmHg(5),1);
    //  // Serial.print(" PSI: ");
    //  // Serial.println(press_sens.psi(), 2);
    // } else {
    //   Serial.println("Pressure sensor not found.");
    // };
#endif
    delay(3000);
  } else {
    tft.DisplayOff();
  };
  //-------------------------------

  //delay(10000);
  //tft.DisplayOff();
  //delay(10000);
  //   if (millis() - timer >= 5000) {
  //     timer = millis();  // таймер на millis() сброс
  //     switch (codeOper) {
  //       case 0:
  //         {
  //           tft.DisplaySleepOn();
  //           //DisplayOff();
  //           codeOper++;
  //           break;
  //         };
  //       case 1:
  //         {
  //           //DisplayOn();
  //           tft.DisplaySleepOff();
  // };
}
