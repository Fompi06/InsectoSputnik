void heatStates(void) {
  if (BatTemp < 15 && !ManCtrl && !digitalRead(BatHeat)) {
    BatCounter++;
    if (BatCounter > 5) {
      digitalWrite(BatHeat, 1);
      loraSend("4, 1,");
      loraSendln(digitalRead(CamHeat));
      logln("BatHeat: On");
      digitalWrite(37, 1);
    }
  } else if (!ManCtrl && BatTemp > 22 && digitalRead(BatHeat)) {
    BatCounter = 0;
    digitalWrite(BatHeat, 0);
    loraSend("4, 0,");
    loraSendln(digitalRead(CamHeat));
    logln("BatHeat: Off");
    digitalWrite(37, 0);
  } else BatCounter = 0;

  if (CamTemp < 20 && !ManCtrl && !digitalRead(CamHeat)) {
    CamCounter++;
    if (CamCounter > 5) {
      digitalWrite(CamHeat, 1);
      loraSend("4,");
      loraSend(digitalRead(BatHeat));
      loraSendln(",1");
      logln("CamHeat: On");
      digitalWrite(36, 1);
    }
  } else if (!ManCtrl && CamTemp > 25 && digitalRead(CamHeat)) {
    CamCounter = 0;
    digitalWrite(CamHeat, 0);
    loraSend("4,");
    loraSend(digitalRead(BatHeat));
    loraSendln(",0");
    logln("CamHeat: Off");
    digitalWrite(36, 0);
  } else CamCounter = 0;

  if(PhotoEn || alt > 300) 
    digitalWrite(PHOTOPIN, 1);
  else 
    digitalWrite(PHOTOPIN, 0);
}