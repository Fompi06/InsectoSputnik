/*
README:
Данный код использовался для прошивки Arduino Mega 2560 для участия во Всероссийском конкурсе по запуску стратоспутников.
Большинство кода не имеет комментирования ввиду сильноограниченного времени на разработку кода.
Если у вас возникли вопросы по поводу кода, как он работает, что делает та или иная функция/команда,
то напишите мне в социальные сети или откройте Issue с тегом question.
Данный код был написан на скорую руку, поэтому при повторном его использовании в проектах нуждается в возможном частичном/полном рефакторинге.
Данный код возможно не будет нигде использоваться, кроме использования в следующих соревнованиях по стратоспутникам.
Данный код может свободно использоваться любым человеком для своих целей.
Человек, написавший данный код: Герман Хайруллин.
©Команда Инсектоспутники
*/

#include <GParser.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <GyverWDT.h>
#include <MS5611.h>
#include <DallasTemperature.h>

#define DS_PIN 10  // пин для термометров
#define ONE_WIRE_BUS 10
#define CamHeat 2
#define BatHeat 11
#define V5 A12
#define V3 A13
#define VBat A14
#define Amp A11
#define PHOTOPIN 3

// #define DEBUG_EN
#ifdef DEBUG_EN
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#else
#define DEBUG(x)
#define DEBUGLN(x)
#endif

#define LOG_EN
#ifdef LOG_EN
#define logln(x) LoggerSerial.println(x)
#else
#define logln(x)
#endif

#define GPSSerial Serial3
#define LoggerSerial Serial2
#define LoraSerial Serial1

SoftwareSerial ESPSerial(6, 5);

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// создаём указатель массив для хранения адресов датчиков
DeviceAddress *sensorsUnique;
// количество датчиков на шине
int countSensors = 3;
// MS5x barometer(&Wire);
MS5611 MS5611(0x77);

double alt = 0;
double altRel = 0;
float pres = 0;
double altStart = 0;
float Temp = 0;
float BatTemp = 0;
float CamTemp = 0;
float barTemp = 0;
byte BatCounter = 0;
byte CamCounter = 0;
byte GCounter = 0;
int time[4];
int date[3];
double GLati = 0;
double GLong = 0;
float GAlt = 0;
int GSputniks = 0;
float BatVolt = 0;
uint16_t n = 1;

uint32_t timer = millis();
uint32_t timerTemp = millis();
uint32_t timerBar = millis();
uint32_t timerPower = millis();
uint32_t logTimer = millis();
uint32_t barTimer = millis();
uint32_t GsendTimer = millis();
uint32_t heatTimer = millis();

bool ManCtrl = 0;
bool EcoMode = 0;
bool PhotoEn = 0;
bool photoFlag = 0;
String dataString = "";


void setup(void) {

  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  sensors.begin();
  sensorsUnique = new DeviceAddress[countSensors];
  for (int i = 0; i < countSensors; i++) {
    sensors.getAddress(sensorsUnique[i], i);
  }
  for (int i = 0; i < countSensors; i++) {
    sensors.setResolution(sensorsUnique[i], 12);
  }
  Serial.begin(115200);
  ESPSerial.begin(9600);
  pinMode(CamHeat, OUTPUT);
  pinMode(BatHeat, OUTPUT);
  pinMode(PHOTOPIN, OUTPUT);
  pinMode(37, OUTPUT); // для дебаггинга
  pinMode(36, OUTPUT); // для дебаггинга
  pinMode(34, OUTPUT); // для дебаггинга
  digitalWrite(BatHeat, 0);
  digitalWrite(CamHeat, 0);
  pinMode(V3, INPUT);
  pinMode(V5, INPUT);
  pinMode(VBat, INPUT);
  pinMode(Amp, INPUT);
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ);
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
  LoraSerial.begin(9600);
  LoggerSerial.begin(9600);
  // Ask for firmware version
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  logln("Starting...");
  logln("Mode,millis(),hour,minute,seconds,Temp,BatTemp,CamTemp,BarTemp,pres,alt,altRel,Ghour,Gminute,Gsecond,Gday,Gmonth,Gyear,GLati,GLong,GAlt,GSputniks,BatVolt,BatHeat,CamHeat,CameraEn");
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024);  // Режим сторжевого сброса , таймаут ~8сs
}

template<typename T1>
void loraSend(T1 val) {
  if (!EcoMode)
    LoraSerial.print(val);
}
template<typename T2>
void loraSendln(T2 val) {
  if (!EcoMode)
    LoraSerial.println(val);
}
template<typename T3>
void log(T3 val, char n = '0') {
  LoggerSerial.print(val);
  if (n != '0') LoggerSerial.print(n);
}

// функции-костыли для решения проблемы с частотой отправки GPS-данных по LoRa-модулю
template<typename T4>
void GloraSend(T4 val) {
  if (!EcoMode && GCounter == 4)
    LoraSerial.print(val);
}
template<typename T5>
void GloraSendln(T5 val) {
  if (!EcoMode && GCounter == 4)
    LoraSerial.println(val);
}

void logTime() {
  for (int i = 0; i < 3; i++) {
    log(time[i], ',');
  }
}

void loop(void) {
  parsing();
  checkTemp();
  checkBarometer();
  parseGPS();
  checkGPS();
  sendpowerStates();
  Watchdog.reset();
}