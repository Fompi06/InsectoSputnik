void checkBarometer() {
  if (millis() - barTimer > 5000) {
    barTimer = millis();
    MS5611.setOversampling(OSR_ULTRA_HIGH);
    int result = MS5611.read();
    if (result != MS5611_READ_OK) {
      DEBUG("Error in read: ");
      DEBUGLN(result);
      loraSendln("1,404,404,404,404");
      barTemp = 404;
      pres = 404;
      alt = 404;
    } else {
      barTemp = MS5611.getTemperature();
      pres = MS5611.getPressure() * 100;
      alt = getAltitude(0, pres, barTemp);
      loraSend("1,");
      loraSend(barTemp);
      loraSend(',');
      loraSend(pres);
      loraSend(',');
      loraSend(alt);
      if (altStart == 0) altStart = alt;
      loraSend(",");
      altRel = alt - altStart;
      loraSendln(altRel);
    }
    savelog(1);
  }
}

double getAltitude(bool temperatureCorrected, float _PRESS, float temperature) {
  // Данная функция взята из библиотеки MS5x
  if (_PRESS == 0) {
    return -1;  // Pressure hasn't been initailized yet, return -1
  }

  // Temperature correction for the altitude can create a lot of wobble to altitude calculations
  // make sure you do some averaging of temperature to prevent random spikes from creating huge
  // swings in altitude.
  if (temperatureCorrected) {
    if (temperature == 0) {
      return -2;  // Temperature has not been intialized yet return -2
    } else {
      return ((pow(101235 / (_PRESS), 0.19022256) - 1.) * temperature) / 0.0065;
    }
  } else {
    return 44330. * (1. - pow(_PRESS / 101235, 0.19029495));
  }
}