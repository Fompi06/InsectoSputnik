void savelog(void) {
  if (millis() - logTimer > 1000) {
    logTimer = millis();
    String data = String(millis());
    data += ",";
    data += String(Temp);
    data += " ";
    data += String(BatTemp);
    data += " ";
    data += String(CamTemp);
    data += " Barometer: ";
    data += String(prevTemperature);
    data += " ";
    data += String(pres);
    data += " ";
    data += String(alt);
    data += " ";
    data += String(altRel);
    data += " ";
    data += String(camBar);
    data += " Time: ";
    for (int i = 0; i < 4; i++) {
      if (time[i] < 9) data += "0";
      data += String(time[i]);
      data += ":";
    }
    data += " Date: ";
    for (int i = 0; i < 3; i++) {
      if (i == 2) data += "20";
      if (date[i] < 9) data += "0";
      data += String(date[i]);
      data += "/";
    }
    data += " BatteryVoltage:";
    data += BatVolt;
    LoggerSerial.println(data);
  }
}