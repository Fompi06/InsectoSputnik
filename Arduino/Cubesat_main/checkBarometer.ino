void checkBarometer(void) {
  barometer.checkUpdates();
  if (barometer.isReady()) {
    double pressure;
    double temperature;
    temperature = barometer.GetTemp();  // Returns temperature in C
    pressure = barometer.GetPres();     // Returns pressure in Pascals
    // delay(250);
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