
#include <GParser.h>
#include <Wire.h>
#include <MS5x.h>
#include <microDS18B20.h>
#include <Adafruit_GPS.h>

#define DS_PIN 10  // пин для термометров
#define CamHeat 2
#define BatHeat 11
#define V5 A12
#define V3 A13
#define VBat A14
#define Amp A11


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
#define log(x) LoggerSerial.print(x)
#define logln(x) LoggerSerial.println(x)
#else
#define log(x)
#define logln(x)
#endif

#define GPSSerial Serial3
#define LoggerSerial Serial2
#define LoraSerial Serial

/*
Отправка:
0 (temp) - temp, tempBat, tempCam (позже)
1 (i2c) - tempBar, pres, alt (позже), altRel
2 (GPS) - h, m, s, d, m, y, Lat (широта), Long (долгота), Alt, Antenna status
3 (V, I) - BatV, 5V - V, 3.3V - V, amper
4 (BatHeat, CamHeat)
*/

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint8_t temp_addr[] = { 0x28, 0xFE, 0x25, 0x22, 0xA1, 0x22, 0x8, 0xED };
uint8_t tempBat_addr[] = { 0x28, 0xEE, 0x1, 0x1D, 0x0, 0x0, 0x0, 0x4C };
uint8_t tempCam_addr[] = { 0x28, 0x20, 0x98, 0x96, 0xF0, 0x1, 0x3C, 0x9D };
MicroDS18B20<DS_PIN, temp_addr> temp;        // Создаем термометр с адресацией
MicroDS18B20<DS_PIN, tempBat_addr> tempBat;  // Создаем термометр с адресацией
MicroDS18B20<4, tempCam_addr> tempCam;  // Создаем термометр с адресацией

MS5x barometer(&Wire);

uint32_t prevTime;  // The time, in MS the device was last polled

double prevPressure = 0;     // The value of the pressure the last time the sensor was polled
double prevTemperature = 0;  // The value of the temperature the last time the sensor was polled
double seaLevelPressure = 0;
double alt = 0;
double altRel = 0;
double altStart = 0;
float BatTemp = 0;
byte BatCounter = 0;
float CamTemp = 0;
byte CamCounter = 0;

uint32_t timer = millis();
uint32_t timerTemp = millis();
uint32_t timerBar = millis();

bool ManCtrl = 0;
bool EcoMode = 0;

void setup(void) {

  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  // Serial.begin(115200);
  pinMode(CamHeat, OUTPUT);
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
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
  LoraSerial.begin(9600);
  LoggerSerial.begin(9600);
  while (barometer.connect() > 0) {  // barometer.connect starts wire and attempts to connect to sensor
    DEBUGLN(F("Error connecting..."));
    delay(500);
  }
  barometer.setDelay(1000);  // barometer will wait 250 ms before taking new temperature and pressure readings
  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
  logln("Starting...");
  
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

/*
Прием:
0 - ручное управление (ManCtrl)
1 - ручное управление подогревом
2 - записать в логгер сообщение
3 - Перепрошивка контроллера
4 - проверка связи со вторым приемником
*/
void parsing(void) {
  if (LoraSerial.available() > 0) {
    char buf[50];
    LoraSerial.readBytesUntil(';', buf, 50);
    GParser data(buf, ',');
    int ints[10];
    data.parseInts(ints);

    switch (ints[0]) {
      case 0:
        if (data[1]) {
          ManCtrl = 1;
          BatCounter = 0;
          CamCounter = 0;
        } else ManCtrl = 0;
        break;
      case 1:
        if (data[1] && ManCtrl) digitalWrite(CamHeat, 1);
        else digitalWrite(CamHeat, 0);
        if (data[2] && ManCtrl) digitalWrite(BatHeat, 1);
        else digitalWrite(BatHeat, 0);
        if (data[3]) EcoMode = 1;
        else EcoMode = 0;
        break;
      case 2:
        for (int i = 0; i < 50; i++)
          log(buf[i]);
        logln("");
        break;

      case 3:
        // TODO: перепрошивка контроллера
        break;
      case 4:
        loraSendln("5,1");
        break;
    }
  }
}
// 5, значение барометра;
// 6, сообщение об успешной перепрошивке;
void checkTemp(void) {
  if (millis() - timerTemp > 2000) {
    timerTemp = millis();
    loraSend("0,");
    log("Temps: ")
      DEBUG("t: ");
    if (temp.readTemp()) {
      float temperature = temp.getTemp();
      DEBUGLN(temperature);
      loraSend(temperature);
      log(temperature);

    } else {
      DEBUGLN("404");
      loraSend("404");
      log("404");
    }
    loraSend(", ");
    log(", ");
    DEBUG("tBat: ");
    if (tempBat.readTemp()) {
      float temperature = tempBat.getTemp();
      BatTemp = temperature;
      DEBUGLN(temperature);
      loraSend(temperature);
      log(temperature);
    } else {
      DEBUGLN("404");
      loraSend("404");
      BatTemp = 404;
    }
    loraSend(", ");
    log(", ");
    DEBUG("tCam: ");
    if (tempCam.readTemp()) {
      float temperature = tempCam.getTemp();
      CamTemp = temperature;
      DEBUGLN(temperature);
      loraSendln(temperature);
      logln(temperature);
    } else {
      CamTemp = 404;
      DEBUGLN("404");
      loraSendln("404");
      logln("404");
    }
    temp.requestTemp();
    tempBat.requestTemp();
    tempCam.requestTemp();
  }
}

void heatStates(void) {
  if (BatTemp < 15 && !ManCtrl && !digitalRead(BatHeat)) {
    BatCounter++;
    if (BatCounter > 3) {
      digitalWrite(BatHeat, 1);
      loraSend("4, 1, ");
      loraSendln(digitalRead(CamHeat));
      logln("BatHeat: On");
    }
  } else if (!ManCtrl && BatTemp > 22 && digitalRead(BatHeat)) {
    BatCounter = 0;
    digitalWrite(BatHeat, 0);
    loraSend("4, 0, ");
    loraSendln(digitalRead(CamHeat));
    logln("BatHeat: Off");
  } else BatCounter = 0;

  if (CamTemp < 20 && !ManCtrl && !digitalRead(CamHeat)) {
    CamCounter++;
    if (CamCounter > 3) {
      digitalWrite(CamHeat, 1);
      loraSend("4, ");
      loraSend(digitalRead(BatHeat));
      loraSendln("1");
      logln("CamHeat: On");
    }
  } else if (!ManCtrl && CamTemp > 25 && digitalRead(CamHeat)) {
    CamCounter = 0;
    digitalWrite(CamHeat, 0);
    loraSend("4, ");
    loraSend(digitalRead(BatHeat));
    loraSendln("0");
    logln("CamHeat: Off");
  } else CamCounter = 0;
}

void checkBarometer(void) {
  barometer.checkUpdates();
  if (barometer.isReady()) {
    double pressure;
    double temperature;
    temperature = barometer.GetTemp();  // Returns temperature in C
    pressure = barometer.GetPres();     // Returns pressure in Pascals
    delay(250);
    DEBUG(F("tBar:  "));
    DEBUGLN(temperature);
    DEBUG(F("pres: "));
    DEBUGLN(pressure);
    if ((temperature != prevTemperature) || (pressure != prevPressure)) {

      // Calculate predicted seaLevel pressure based off a known altitude in meters
      if (seaLevelPressure == 0) seaLevelPressure = barometer.getSeaLevel(217.3);



      // Calculate current altitude based on pressure reading
      alt = barometer.getAltitude();
      if (altStart == 0)
        altStart = alt;
      altRel = alt - altStart;

      DEBUG(F("alt: "));
      DEBUG(alt);
      DEBUGLN(F(" meters"));
      DEBUG(F("altRel: "));
      DEBUG(altRel);
      DEBUGLN(F(" meters"));

      prevTemperature = temperature;
      prevPressure = pressure;
      loraSend("1, ");
      loraSend(temperature);
      loraSend(", ");
      loraSend(pressure);
      loraSend(", ");
      loraSend(alt);
      loraSend(", ");
      loraSendln(altRel);
      log("Barometr: ");
      log(temperature);
      log(", ");
      log(pressure);
      log(", ");
      logln(alt);
    }
  }
}

void parseGPS(void) {
#ifdef DEBUG_EN
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) DEBUG(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    DEBUG(GPS.lastNMEA());           // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another
  }
#endif
}

void checkGPS(void) {
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis();  // reset the timer
    loraSend("2, ");
    DEBUG("\nTime: ");
    log("Time: ");
    if (GPS.hour < 10) {
      DEBUG('0');
      loraSend('0');
      log('0');
    }
    #ifdef LOG_EN
    LoggerSerial.print(GPS.hour, DEC);
    #endif
    log(':');
    /*Serial.print(GPS.hour, DEC);*/ DEBUG(':');
    
    if (!EcoMode) LoraSerial.print(GPS.hour, DEC);
    loraSend(", ");
    if (GPS.minute < 10) {
      DEBUG('0');
      loraSend('0');
      log('0');
    }
    #ifdef LOG_EN
    LoggerSerial.print(GPS.minute, DEC);
    #endif
    log(':');
    /*Serial.print(GPS.minute, DEC);*/ DEBUG(':');
    if (!EcoMode) LoraSerial.print(GPS.minute, DEC);
    loraSend(", ");
    if (GPS.seconds < 10) {
      DEBUG('0');
      loraSend('0');
      log('0');
    }
    #ifdef LOG_EN
    LoggerSerial.print(GPS.seconds, DEC);
    #endif
    /*Serial.print(GPS.seconds, DEC);*/ DEBUG('.');
    if (!EcoMode) LoraSerial.print(GPS.seconds, DEC);
    loraSend(", ");
    if (GPS.milliseconds < 10) {
      DEBUG("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      DEBUG("0");
    }
    DEBUGLN(GPS.milliseconds);
    DEBUG("Date: ");
    log(" Date: ");
    #ifdef LOG_EN
    LoggerSerial.print(GPS.day, DEC);
    #endif
    log('/');
    /*Serial.print(GPS.day, DEC);*/ DEBUG('/');
    
    if (!EcoMode) LoraSerial.print(GPS.day, DEC);
    loraSend(",");
    #ifdef LOG_EN
    LoggerSerial.print(GPS.month, DEC);
    #endif
    log('/20');
    /*Serial.print(GPS.month, DEC);*/ DEBUG("/20");
    if (!EcoMode) LoraSerial.print(GPS.month, DEC);
    loraSend(",20");
    #ifdef LOG_EN
    LoggerSerial.print(GPS.year, DEC);
    #endif
    log(" Location: ");
    /*Serial.println(GPS.year, DEC);*/
    if (!EcoMode) LoraSerial.print(GPS.year, DEC);
    loraSend(",");
    DEBUG("Fix: ");
    DEBUG((int)GPS.fix);
    DEBUG(" quality: ");
    DEBUGLN((int)GPS.fixquality);
    if (GPS.fix) {
      DEBUG("Location: ");
      /*Serial.print(GPS.latitude, 4);*/ DEBUG(GPS.lat);
      DEBUG(",");
      if (!EcoMode) LoraSerial.print(GPS.latitude, 4);
      loraSend(",");
      /*Serial.print(GPS.longitude, 4);*/ DEBUGLN(GPS.lon);
      if (!EcoMode) LoraSerial.print(GPS.longitude, 4);
      loraSend(",");
      DEBUG("Speed (knots): ");
      DEBUGLN(GPS.speed);
      DEBUG("Angle: ");
      DEBUGLN(GPS.angle);
      DEBUG("Altitude: ");
      DEBUGLN(GPS.altitude);
      loraSendln(GPS.altitude);
      loraSend(",");
      DEBUG("Satellites: ");
      DEBUGLN((int)GPS.satellites);
      DEBUG("Antenna status: ");
      DEBUGLN((int)GPS.antenna);
      loraSendln((int)GPS.antenna);
    } else {
      loraSendln("0,0,0,0");
    }
  }
}

void loop(void) {

  parsing();
  checkTemp();
  heatStates();
  checkBarometer();
  parseGPS();
  checkGPS();
}