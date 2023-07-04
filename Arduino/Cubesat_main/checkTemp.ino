void checkTemp(void) {
  if (millis() - timerTemp > 2000) {
    timerTemp = millis();
    loraSend("0,");
    DEBUG("t: ");
    log("0,");
    log(millis());
    log(",");
    logTime();
    if (temp.readTemp()) {
      float temperature = temp.getTemp();
      DEBUGLN(temperature);
      loraSend(temperature);
      Temp = temperature;
      log(temperature);
    } else {
      DEBUGLN("404");
      loraSend("404");
      log(404);
      Temp = 404;
    }
    loraSend(",");
    log(",");
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
      log(404);
    }
    loraSend(",");
    log(",");
    DEBUG("tCam: ");
    if (tempCam.readTemp()) {
      float temperature = tempCam.getTemp();
      CamTemp = temperature;
      DEBUGLN(temperature);
      loraSendln(temperature);
      log(temperature);
    } else {
      CamTemp = 404;
      DEBUGLN("404");
      loraSendln("404");
      log(404);
    }
    logln();
    temp.requestTemp();
    tempBat.requestTemp();
    tempCam.requestTemp();
  }
}