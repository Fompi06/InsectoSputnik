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
      BatTemp = 50;
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
      CamTemp = 50;
      DEBUGLN("404");
      loraSendln("404");
      logln("404");
    }
    temp.requestTemp();
    tempBat.requestTemp();
    tempCam.requestTemp();
  }
}