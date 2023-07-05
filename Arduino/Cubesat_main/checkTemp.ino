void checkTemp(void) {
  temp.requestTemp();
  tempBat.requestTemp();
  tempCam.requestTemp();
  if (millis() - timerTemp > 5000) {
    timerTemp = millis();
    loraSend("0,");
    DEBUG("t: ");
    if (temp.readTemp()) {
      float temperature = temp.getTemp();
      DEBUGLN(temperature);
      loraSend(temperature);
      Temp = temperature;
    } else {
      DEBUGLN("404");
      loraSend("404");
      Temp = 404;
    }
    loraSend(",");
    DEBUG("tBat: ");
    if (tempBat.readTemp()) {
      float temperature = tempBat.getTemp();
      BatTemp = temperature;
      DEBUGLN(temperature);
      loraSend(temperature);
    } else {
      DEBUGLN("404");
      loraSend("404");
      BatTemp = 404;
    }
    loraSend(",");
    DEBUG("tCam: ");
    if (tempCam.readTemp()) {
      float temperature = tempCam.getTemp();
      CamTemp = temperature;
      DEBUGLN(temperature);
      loraSendln(temperature);
    } else {
      CamTemp = 404;
      DEBUGLN("404");
      loraSendln("404");
    }
    savelog(0);
  }
  
}