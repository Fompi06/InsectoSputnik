/*
Прием:
0 - ручное управление (ManCtrl)
1 - ручное управление подогревом
2 - записать в логгер сообщение
3 - Перепрошивка контроллера
4 - проверка связи с базой
5 - значение барометра;
6 - сообщение об успешной перепрошивке;
7 - получение времени
*/

void parsing(void) {
  if (LoraSerial.available() > 0) {
    char buf[50];
    LoraSerial.readBytesUntil(';', buf, 50);
    GParser data(buf, ',');
    int ints[10];
    data.parseInts(ints);

    switch (ints[0]) {
      case 0:
        if (ints[1]) {
          ManCtrl = 1;
          BatCounter = 0;
          CamCounter = 0;
        } else ManCtrl = 0;
        break;
      case 1:
        if (ints[1] && ManCtrl) {
          digitalWrite(CamHeat, 1);
          digitalWrite(36, 1);
        } else if (ManCtrl) {
          digitalWrite(CamHeat, 0);
          digitalWrite(36, 0);
        }
        if (ints[2] && ManCtrl) {
          digitalWrite(BatHeat, 1);
          digitalWrite(37, 1);
        } else if (ManCtrl) {
          digitalWrite(BatHeat, 0);
          digitalWrite(37, 0);
        }
        if (ints[3]) EcoMode = 1;
        else EcoMode = 0;
        if (ints[4] && ManCtrl) PhotoEn = 1;
        else if (ManCtrl) PhotoEn = 0;
        break;
      case 2:
        for (int i = 0; i < 50; i++)
          log(buf[i]);
        logln("");
        break;

      case 3:
        // TODO: перепрошивка контроллера
        ESPSerial.println(1);
        break;
      case 4:
        loraSendln("5,1");
        break;
      case 5:
        // TODO: прием барометра (отменен)
        break;
      case 6:
        // TODO: сообщение об успешной перепрошивке и отправка сообщения по радиосвязи и в логгер
        DEBUG(ints[1]);
        DEBUG("    ");
        DEBUGLN(ints[2]);
        log(millis(), ',');
        logTime();
        log("booting:");
        log()
        break;
      case 7:
        for (int i = 0; i < 3; i++) {
          date[i] = ints[i + 1];
          time[i] = ints[i + 5];
        }
        break;
    }
  }
}
