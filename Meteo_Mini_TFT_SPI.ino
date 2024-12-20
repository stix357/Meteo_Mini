#include "MINI_TFT_SPI.h"
#include "SHT21.h"
#include "HX710B.h"

#define heightChar 8
#define btnPin 5

//-----------------
#define SERIALDEBUG
//----------------

Mini_TFT_SPI tft;
SHT21 sht;
HX710B press_sens;

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
  //Serial.print(F("Size CharMap= "));
  // Serial.println(sizeof(CharMap));
#endif
  // setup code
  pinMode(btnPin, INPUT);
  Wire.begin();
  tft.Init();
  tft.Rotate(0);
  tft.scale = 3;
  tft.ClearDisplay();
  tft.DisplayOn();
  //----------------------
  tft.SetColor(255, 150, 0);
  tft.MoveTo(60, ysize - heightChar * tft.scale);
  tft.DrawByteIcon7x7_meteo(1);
  //---------------------------
  tft.SetColor(0, 70, 255);
  tft.MoveTo(60, ysize - heightChar * 2 * tft.scale - 2);
  tft.DrawByteIcon7x7_meteo(2);
  //----------------------
  tft.SetColor(255, 255, 255);
  tft.MoveTo(60, ysize - heightChar * 3 * tft.scale - 4);
  tft.DrawByteIcon7x7_meteo(3);
  //----------------------------
  //press.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  press_sens.begin(7, 6, 128);
  // delay(1000);
  // #ifdef SERIALDEBUG
  // if (press_sens.is_ready()) {
  //   Serial.print("HX710 reading: ");
  //   Serial.print("Pascal: ");
  //   Serial.println(press_sens.pascal(),2);
  //   Serial.print("ATM: ");
  //   Serial.println(press_sens.atm(),2);
  //   Serial.print("mmHg: ");
  //   Serial.println(press_sens.mmHg(),2);
  //   Serial.print("PSI: ");
  //   Serial.println(press_sens.psi(),2);
  // } else {
  //   Serial.println("Pressure sensor not found.");
  // };
  //Serial.println(press_sens.read());
  //#endif
  //---------------------------
  delay(3000);
  tft.ClearDisplay();
  tft.MoveTo(0, 0);
  tft.DisplayOff();
  timer30s = millis();
}

void loop() {
  if (digitalRead(btnPin) == 1) {
    timer30s = millis();
  };
  //---------------------------
  if (millis() - timer30s < 30000) {  // таймер timer30s
    temp = sht.getTemperature();
    humidity = sht.getHumidity() + 11;
    press = press_sens.mmHg() + 1120;
    //------------------temp
    String sensorT(temp, 1);
    sensorT.toCharArray(sensorPrintout, 5);
    //tft.Rotate(0);
    //tft.scale = 3;
    tft.MoveTo(0, ysize - heightChar * tft.scale);
    tft.SetColor(255, 150, 0);
    tft.DrawByteIcon7x7_meteo(1);
    //tft.PrintTFTRus(F("T "));
    tft.MoveTo(30, ysize - heightChar * tft.scale);
    tft.SetColor(0, 255, 0);
    tft.PrintTFTRus(sensorPrintout);
    tft.PrintTFTRus(F(" "));
    //tft.DrawByteIcon7x7(86);
    tft.DrawByteIcon7x7_meteo(0);
    //---------------------------------humidity
    tft.MoveTo(0, ysize - heightChar * 2 * tft.scale - 2);
    tft.SetColor(0, 70, 255);
    tft.DrawByteIcon7x7_meteo(2);
    tft.MoveTo(30, ysize - heightChar * 2 * tft.scale - 2);
    String sensorH(humidity, 1);
    sensorH.toCharArray(sensorPrintout, 5);
    //tft.PrintTFTRus(F("B "));
    tft.PrintTFTRus(sensorPrintout);
    tft.PrintTFTRus(F(" %"));
    //-----------------------------------press
    tft.SetColor(255, 255, 255);
    tft.MoveTo(0, ysize - heightChar * 3 * tft.scale - 4);
    tft.DrawByteIcon7x7_meteo(3);
    tft.MoveTo(30, ysize - heightChar * 3 * tft.scale - 4);
    String sensorP(press, 0);
    sensorP.toCharArray(sensorPrintout, 5);
    //tft.PrintTFTRus(F("Д "));
    tft.PrintTFTRus(sensorPrintout);
    tft.PrintTFTRus(F(" мм"));
    tft.DisplayOn();
    //---------------------------------
#ifdef SERIALDEBUG
    Serial.print("Temp: ");  // print readings
    Serial.print(temp, 2);
    Serial.print("\t Humidity: ");
    Serial.println(humidity, 2);
    if (press_sens.is_ready()) {
      Serial.print("HX710 reading: ");
      Serial.print("Pascal: ");
      Serial.println(press_sens.pascal(), 2);
      Serial.print("ATM: ");
      Serial.println(press_sens.atm(), 2);
      Serial.print("mmHg: ");
      Serial.println(press_sens.mmHg(), 2);
      Serial.print("PSI: ");
      Serial.println(press_sens.psi(), 2);
    } else {
      Serial.println("Pressure sensor not found.");
    };
#endif
    delay(2000);
  } else {
    tft.DisplayOff();
    ;
  }
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
