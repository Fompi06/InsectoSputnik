void parseGPS(void) {

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
}

void checkGPS(void) {
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis();  // reset the timer
    loraSend("2,");
    DEBUG("\nTime: ");
    log("Time: ");
    if (GPS.hour < 10) {
      DEBUG('0');
      loraSend('0');
      log("0");
    }
#ifdef LOG_EN
     dataString += String((GPS.hour, DEC));
#endif
    log(":");
    /*Serial.print(GPS.hour, DEC);*/ DEBUG(':');

    if (!EcoMode) LoraSerial.print(GPS.hour, DEC);
    loraSend(",");
    if (GPS.minute < 10) {
      DEBUG('0');
      loraSend('0');
      log("0");
    }
#ifdef LOG_EN
    dataString += String((GPS.minute, DEC));
#endif
    log(':');
    /*Serial.print(GPS.minute, DEC);*/ DEBUG(':');
    if (!EcoMode) LoraSerial.print(GPS.minute, DEC);
    loraSend(",");
    if (GPS.seconds < 10) {
      DEBUG('0');
      loraSend('0');
      log('0');
    }
#ifdef LOG_EN
    dataString += String(GPS.seconds, DEC);
#endif
    /*Serial.print(GPS.seconds, DEC);*/ DEBUG('.');
    if (!EcoMode) LoraSerial.print(GPS.seconds, DEC);
    loraSend(",");
    if (GPS.milliseconds < 10) {
      DEBUG("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      DEBUG("0");
    }
    DEBUGLN(GPS.milliseconds);
    DEBUG("Date: ");
    log(" Date: ");
#ifdef LOG_EN
    dataString += String(GPS.day, DEC);
#endif
    log('/');
    /*Serial.print(GPS.day, DEC);*/ DEBUG('/');

    if (!EcoMode) LoraSerial.print(GPS.day, DEC);
    loraSend(",");
#ifdef LOG_EN
   dataString += String(GPS.month, DEC);
#endif
    log("/20");
    /*Serial.print(GPS.month, DEC);*/ DEBUG("/20");
    if (!EcoMode) LoraSerial.print(GPS.month, DEC);
    loraSend(",20");
#ifdef LOG_EN
   dataString += String(GPS.year, DEC);
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
#ifdef LOG_EN
      dataString += String((GPS.latitude, 4));
      log(", ");
      dataString += String((GPS.longitude, 4));
      log(", ");
      logln(GPS.altitude);
#endif      
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
      loraSend(GPS.altitude);
      loraSend(",");
      DEBUG("Satellites: ");
      DEBUGLN((int)GPS.satellites);
      DEBUG("Antenna status: ");
      DEBUGLN((int)GPS.antenna);
      loraSendln((int)GPS.antenna);
    } else {
      loraSendln("0,0,0,0");
      logln("0,0,0");
    }
  }
}