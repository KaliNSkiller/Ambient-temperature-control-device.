//********************Temp Control Relay
int PinRelay = 5;
//********************Display
int PinClkLcd = 2;
int PinDioLcd = 3;
#include <TM1637.h>
#define CLK PinClkLcd
#define DIO PinDioLcd
TM1637 Display1(CLK, DIO);
//********************Temp
int PinVccTemp = 8;
int PinDataTemp = 9;
int PinGndTemp = 10;
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS PinDataTemp
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature Temp(&oneWire);
int8_t Digitos[] = {8, 8, 8, 8};
int Temp_IN;
int TempSettings = 33;
//********************Switch
int PinSW = 12;
int TimeSW;
int TimeDisplay;
boolean Key;

void setup() {
  Temp.begin();
  Display1.set(7);
  Display1.init();
  Display1.display(Digitos);
  Display1.point(POINT_OFF);
  pinMode(PinSW, INPUT_PULLUP);
  pinMode(PinRelay, OUTPUT);
  pinMode(PinVccTemp, OUTPUT);
  pinMode(PinGndTemp, OUTPUT);
  digitalWrite(PinRelay, HIGH);
  digitalWrite(PinVccTemp, HIGH);
  Temp.requestTemperatures();
  delay(1000);
}

void loop() {
  Temp.requestTemperatures();
  Temp_IN = Temp.getTempCByIndex(0);
  int8_t Digit2 = Temp_IN % 10 ;
  int8_t Digit1 = (Temp_IN % 100) / 10 ;
  Digitos[3] = 12 ;
  Digitos[2] = Digit2 ;
  Digitos[1] = Digit1 ;
  Digitos[0] = 19 ;
  Display1.display(Digitos);
  while (digitalRead(PinSW) == 0) {
    TimeSW++;
    if (TimeSW >= 3000) Settings();
    delay(1);
  }
  TimeSW = 0;
  if (Temp_IN > TempSettings) digitalWrite(PinRelay, LOW);
  if (Temp_IN < TempSettings) digitalWrite(PinRelay, HIGH);
}

void Settings() {
  TimeSW = 0;
  Key = 1;
  while (true) {
    TimeSW++;
    TimeDisplay++;
    int8_t Digit2 = TempSettings % 10 ;
    int8_t Digit1 = (TempSettings % 100) / 10 ;
    if (TimeDisplay  < 100) {
      Digitos[3] = 12 ;
      Digitos[2] = Digit2 ;
      Digitos[1] = Digit1 ;
    } else {
      Digitos[3] = 19 ;
      Digitos[2] = 19 ;
      Digitos[1] = 19 ;
    }
    Digitos[0] = 19 ;
    if (TimeDisplay > 200) TimeDisplay = 0;
    Display1.display(Digitos);
    if (digitalRead(PinSW) == 0 && Key == 0) {
      Key = 1;
      TimeSW = 0;
      TempSettings++;
      if (TempSettings > 99 ) TempSettings = 0;
    }
    if (digitalRead(PinSW) == 1 && Key == 1)Key = 0;
    if (TimeSW >= 3000) break;
    delay(1);
  }
  TimeSW = 0;
}
