void checkTemp() {
  if (millis() - timerTemp > 5000) {
    timerTemp = millis();
    float temperature[3];
    sensors.requestTemperatures();
    Temp = sensors.getTempCByIndex(0);
    BatTemp = sensors.getTempCByIndex(1);
    CamTemp = sensors.getTempCByIndex(2);
    if (Temp == -127) Temp = 404;
    if (BatTemp == -127) BatTemp = 404;
    if (CamTemp == -127) CamTemp = 404;
    // если значение с термометра не пришло
    // (если термометр не отвечает, то функция библиотеки возращает -127, поэтому была сделана корректировка неправильных значений
    // (чтобы обогреватели не считали, что температура термометров -127 и нужно срочно греть))
    loraSend("0,");
    loraSend(Temp);
    loraSend(",");
    loraSend(BatTemp);
    loraSend(",");
    loraSendln(CamTemp);
    savelog(0);
    if (BatTemp < 15 && !ManCtrl && !digitalRead(BatHeat)) {
      BatCounter++;
      if (BatCounter > 5) {
        digitalWrite(BatHeat, 1);
        loraSend("4,1,");
        loraSendln(digitalRead(CamHeat));
        digitalWrite(37, 1);  // для дебаггинга
      }
    } else if (!ManCtrl && BatTemp > 22 && digitalRead(BatHeat)) {
      BatCounter = 0;
      digitalWrite(BatHeat, 0);
      loraSend("4,0,");
      loraSendln(digitalRead(CamHeat));
      digitalWrite(37, 0);  // для дебаггинга
    } else BatCounter = 0;

    if (CamTemp < 20 && !ManCtrl && !digitalRead(CamHeat)) {
      CamCounter++;
      if (CamCounter > 5) {
        digitalWrite(CamHeat, 1);
        loraSend("4,");
        loraSend(digitalRead(BatHeat));
        loraSendln(",1");
        digitalWrite(36, 1);  // для дебаггинга
      }
    } else if (!ManCtrl && CamTemp > 25 && digitalRead(CamHeat)) {
      CamCounter = 0;
      digitalWrite(CamHeat, 0);
      loraSend("4,");
      loraSend(digitalRead(BatHeat));
      loraSendln(",0");
      digitalWrite(36, 0);  // для дебаггинга
    } else CamCounter = 0;

    if (PhotoEn || alt > 300) {
      photoFlag = 1;
      digitalWrite(35, 1);  // для дебаггинга
      digitalWrite(PHOTOPIN, 1);
    } else {
      if (photoFlag) {
        photoFlag = 0;
        digitalWrite(35, 0);  // для дебаггинга
        digitalWrite(PHOTOPIN, 0);
      }
    }
    savelog(4);
  }
}