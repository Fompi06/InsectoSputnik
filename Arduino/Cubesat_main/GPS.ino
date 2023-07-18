void parseGPS() {
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

void checkGPS() {
  if (millis() - timer > 2000) {
    GCounter++;
    GCounter = GCounter % 5;
    timer = millis();
    GloraSend("2,");
    DEBUG("\nTime: ");
    if (GPS.hour < 10) {
      DEBUG('0');
      GloraSend('0');
    }

    // Serial.print(GPS.hour, DEC); DEBUG(':');

    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.hour, DEC);
    GloraSend(",");
    if (GPS.minute < 10) {
      DEBUG('0');
      GloraSend('0');
    }
    // Serial.print(GPS.minute, DEC); DEBUG(':');
    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.minute, DEC);
    GloraSend(",");
    if (GPS.seconds < 10) {
      DEBUG('0');
      GloraSend('0');
    }
    // Serial.print(GPS.seconds, DEC); DEBUG('.');
    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.seconds, DEC);
    GloraSend(",");
    if (GPS.milliseconds < 10) {
      DEBUG("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      DEBUG("0");
    }
    DEBUGLN(GPS.milliseconds);
    DEBUG("Date: ");
    // Serial.print(GPS.day, DEC); DEBUG('/');

    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.day, DEC);
    GloraSend(",");
    // Serial.print(GPS.month, DEC); DEBUG("/20");
    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.month, DEC);
    GloraSend(",20");
    // Serial.println(GPS.year, DEC);
    if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.year, DEC);
    GloraSend(",");
    DEBUG("Fix: ");
    DEBUG((int)GPS.fix);
    DEBUG(" quality: ");
    DEBUGLN((int)GPS.fixquality);
    time[0] = GPS.hour;
    time[1] = GPS.minute;
    time[2] = GPS.seconds;
    time[3] = GPS.milliseconds;
    date[0] = GPS.day;
    date[1] = GPS.month;
    date[2] = GPS.year;
    if (GPS.fix) {
      DEBUG("Location: ");
      // Serial.print(GPS.latitude, 4); DEBUG(GPS.lat);
      DEBUG(",");
      if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.latitude, 4);
      GloraSend(",");
      // Serial.print(GPS.longitude, 4); DEBUGLN(GPS.lon);
      if (!EcoMode && GCounter == 4) LoraSerial.print(GPS.longitude, 4);
      GloraSend(",");
      DEBUG("Speed (knots): ");
      DEBUGLN(GPS.speed);
      DEBUG("Angle: ");
      DEBUGLN(GPS.angle);
      DEBUG("Altitude: ");
      DEBUGLN(GPS.altitude);
      GloraSend(GPS.altitude);
      GloraSend(",");
      DEBUG("Satellites: ");
      DEBUGLN((int)GPS.satellites);
      DEBUG("Antenna status: ");
      DEBUGLN((int)GPS.antenna);
      GloraSendln((int)GPS.satellites);
      GLati = GPS.latitude;
      GLong = GPS.longitude;
      GAlt = GPS.altitude;
      GSputniks = (int)GPS.satellites;
    } else {
      GloraSendln("0,0,0,0");
      GLati = 0;
      GLong = 0;
      GAlt = 0;
      GSputniks = 0;
    }
    savelog(2);
  }
}