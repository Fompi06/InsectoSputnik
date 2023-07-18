void savelog(uint8_t check) {
  static bool flags[5];
  flags[check] = 1;
  bool isReady = 0;
  for (int i = 0; i < 5; i++) {
    isReady = 1;
    if (!flags[i]) {
      isReady = 0;
      break;
    }
  }
  if (isReady) {
    log(millis(), ',');
    logTime();
    log(Temp, ',');
    log(BatTemp, ',');
    log(CamTemp, ',');
    log(barTemp, ',');
    log(pres, ',');
    log(alt, ',');
    log(altRel, ',');
    log(GPS.hour, ',');
    log(GPS.minute, ',');
    log(GPS.seconds, ',');
    log(GPS.day, ',');
    log(GPS.month, ',');
    log(GPS.year, ',');
    log(GLati, ',');
    log(GLong, ',');
    log(GAlt, ',');
    log(GSputniks, ',');
    log(BatVolt, ',');
    log(digitalRead(BatHeat), ',');
    log(digitalRead(CamHeat), ',');
    logln(digitalRead(PHOTOPIN));
    isReady = false;
    for (int i = 0; i < 5; i++) {
      flags[i] = 0;
    }
  }
}