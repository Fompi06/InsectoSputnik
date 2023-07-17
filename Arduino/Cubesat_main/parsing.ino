/*
Прием:
0 - ручное управление (ManCtrl)
1 - ручное управление подогревом
2 - записать в логгер сообщение
3 - Перепрошивка контроллера (не доделано другим членом команды вовремя, поэтому было отменено)
4 - проверка связи с базой
5 - значение барометра (отменено из-за отсуствия смысла);
6 - сообщение об успешной перепрошивке;
7 - получение времени
*/

/*
Отправка:
0 (temp) - temp, tempBat, tempCam (позже)
1 (i2c) - tempBar, pres, alt (позже), altRel
2 (GPS) - h, m, s, d, m, y, Lat (широта), Long (долгота), Alt, Antenna status
3 (V, I) - BatV, 5V - V, 3.3V - V, amper (отменено из-за неисправного датчика)
4 (BatHeat, CamHeat)
5 проверка связи с базой
6 Bar2
*/

void parsing(void) {
  if (LoraSerial.available()) {
    char buf[100];
    LoraSerial.readBytesUntil(';', buf, 100);
    GParser data(buf, ',');
    int ints[10];
    data.parseInts(ints);
    switch (ints[0]) {
      case 0:
        // вкл/выкл ручного управления подогревами и камерой
        if (ints[1]) {
          ManCtrl = 1;
          BatCounter = 0;
          CamCounter = 0;
        } else ManCtrl = 0;
        break;
      case 1:
        // ручное управление подогревами и камерой
        if (ints[1] && ManCtrl) {
          digitalWrite(CamHeat, 1);
          digitalWrite(36, 1);  // для дебаггинга
        } else if (ManCtrl) {
          digitalWrite(CamHeat, 0);
          digitalWrite(36, 0);  // для дебаггинга
        }
        if (ints[2] && ManCtrl) {
          digitalWrite(BatHeat, 1);
          digitalWrite(37, 1);  // для дебаггинга
        } else if (ManCtrl) {
          digitalWrite(BatHeat, 0);
          digitalWrite(37, 0);  // для дебаггинга
        }
        if (ints[3]) EcoMode = 1;
        else EcoMode = 0;
        if (ints[4] && ManCtrl) PhotoEn = 1;
        else if (ManCtrl) PhotoEn = 0;
        break;
      case 2:
        // записать любое сообщение в логгер
        for (int i = 0; i < 50; i++)
          log(buf[i]);
        logln("");
        break;

      case 3:
        // TODO: перепрошивка контроллера (отменено)
        // ESPSerial.println(1);
        break;
      case 4:
        // проверка связи с базой и отправка сигнала обратно
        loraSendln("5,1");
        break;
      case 6:
        // TODO: сообщение об успешной перепрошивке и отправка сообщения по радиосвязи и в логгер(отменено)
        DEBUG(ints[1]);
        DEBUG("    ");
        DEBUGLN(ints[2]);
        log(millis(), ',');
        logTime();
        log("booting:");
        break;
      case 7:
        // Получение даты и времени с базы для корретных значений в логгер
        for (int i = 0; i < 3; i++) {
          date[i] = ints[i + 1];
          time[i] = ints[i + 5];
        }
        break;
    }
  }
}
