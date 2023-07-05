void heatStates(void) {
  if (BatTemp < 15 && !ManCtrl && !digitalRead(BatHeat)) {
    BatCounter++;
    if (BatCounter > 5) {
      digitalWrite(BatHeat, 1);
      loraSend("4, 1,");
      loraSendln(digitalRead(CamHeat));
      digitalWrite(37, 1);
    }
  } else if (!ManCtrl && BatTemp > 22 && digitalRead(BatHeat)) {
    BatCounter = 0;
    digitalWrite(BatHeat, 0);
    loraSend("4, 0,");
    loraSendln(digitalRead(CamHeat));
    digitalWrite(37, 0);
  } else BatCounter = 0;

  if (CamTemp < 20 && !ManCtrl && !digitalRead(CamHeat)) {
    CamCounter++;
    if (CamCounter > 5) {
      digitalWrite(CamHeat, 1);
      loraSend("4,");
      loraSend(digitalRead(BatHeat));
      loraSendln(",1");
      digitalWrite(36, 1);
    }
  } else if (!ManCtrl && CamTemp > 25 && digitalRead(CamHeat)) {
    CamCounter = 0;
    digitalWrite(CamHeat, 0);
    loraSend("4,");
    loraSend(digitalRead(BatHeat));
    loraSendln(",0");
    digitalWrite(36, 0);
  } else CamCounter = 0;

  if (PhotoEn || alt > 300) {
    photoFlag = 1;
    digitalWrite(35, 1);
  } else {
    if (photoFlag) {
      photoFlag = 0;
      digitalWrite(35, 0);
    }
  }
  savelog(4);
}