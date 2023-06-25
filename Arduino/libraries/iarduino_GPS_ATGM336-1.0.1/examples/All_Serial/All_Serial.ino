// ПРИМЕР ВЫВОДИТ ВСЕ ДАННЫЕ:                      //
                                                   //
#include <iarduino_GPS_NMEA.h>                     //  Подключаем библиотеку для расшифровки строк протокола NMEA получаемых по UART.
#include <iarduino_GPS_ATGM336.h>                  //  Подключаем библиотеку для настройки параметров работы GPS модуля ATGM336.
                                                   //
iarduino_GPS_NMEA    gps;                          //  Объявляем объект gps         для работы с функциями и методами библиотеки iarduino_GPS_NMEA.
iarduino_GPS_ATGM336 SettingsGPS;                  //  Объявляем объект SettingsGPS для работы с функциями и методами библиотеки iarduino_GPS_ATGM336.
                                                   //
uint8_t i[30][6];                                  //  Объявляем массив для получения данных о 30 спутниках в формате: {ID спутника (1...255), Отношение сигнал/шум (SNR) в дБ, тип навигационной системы (1-GPS/2-Глонасс/3-Galileo/4-Beidou/5-QZSS), Флаг участия спутника в позиционировании (1/0), Азимут положения спутника (0°...360°), Угол возвышения спутника (0°-горизонт ... 90°-зенит) }.
char* sa[]={"NoName ","GPS    ","Глонасс"};        //  Определяем массив строк содержащих названия навигационных систем спутников.
char* wd[]={"Вс","Пн","Вт","Ср","Чт","Пт","Сб"};   //  Определяем массив строк содержащих по две первых буквы из названий дня недели.
                                                   //
void setup(){                                      //
//   Инициируем работу библиотек:                  //
     Serial.begin(9600);                           //  Инициируем работу с аппаратной шиной UART для вывода данных в монитор последовательного порта на скорости 9600 бит/сек.
     SettingsGPS.begin(Serial1);                   //  Инициируем работу с GPS модулем по указанной шине UART. Функция сама определит текущую скорость GPS модуля ATGM336 (вместо аппаратной шины, можно указывать программную).
     gps.begin(Serial1);                           //  Инициируем расшифровку строк NMEA указав объект используемой шины UART (вместо аппаратной шины, можно указывать программную).
//   Настраиваем работу модуля:                    //
     SettingsGPS.baudrate(9600);                   //  Устанавливаем скорость передачи данных модулем и скорость работы шины UART в 9600 бит/сек.
     SettingsGPS.system(GPS_GP, GPS_GL);           //  Указываем что данные нужно получать от спутников навигационных систем GPS (GPS_GP) и Glonass (GPS_GL).
     SettingsGPS.composition(NMEA_RMC, NMEA_GGA, NMEA_GSA, NMEA_GSV); // Указываем что каждый пакет данных NMEA должен содержать строки идентификаторов NMEA_RMC (рекомендуемый минимум навигационных данных), NMEA_GGA (последнее зафиксированное местоположение), GSA (информация об активных спутниках) и GSV (информация о наблюдаемых спутниках).
     SettingsGPS.updaterate(1);                    //  Указываем обновлять данные 1 раз в секунду. Функция gps.read() читает данные в 2 раза медленнее чем их выводит модуль.
}                                                  //
                                                   //
void loop(){                                       //
     gps.read(i);                                  //  Читаем данные с получением информации о спутниках в массив i (чтение может занимать больше 1 секунды). Если указать второй параметр gps.read(i,true); то в массиве окажутся данные только тех спутников, которые принимают участие в позиционировании.
     /* Время:                                     */  Serial.print(gps.Hours); Serial.print(":"); Serial.print(gps.minutes); Serial.print(":"); Serial.print(gps.seconds); Serial.print(" ");
     /* Дата:                                      */  Serial.print(gps.day  ); Serial.print("."); Serial.print(gps.month  ); Serial.print("."); Serial.print(gps.year   ); Serial.print("г (");
     /* Дополнительные данные даты:                */  Serial.print(wd[gps.weekday]); Serial.print("), UnixTime: "); Serial.print(gps.Unix); Serial.print(". ");
     /* Координаты (широта, долгота, высота):      */  Serial.print("Ш: "); Serial.print(gps.latitude,5); Serial.print("°, Д: "); Serial.print(gps.longitude,5); Serial.print("°, В: "); Serial.print(gps.altitude,1); Serial.print("м. ");
     /* Движение (скорость, курс):                 */  Serial.print("Скорость: "); Serial.print(gps.speed); Serial.print("км/ч, "); Serial.print(gps.course); Serial.print("°. ");
     /* Спутники (активные/наблюдаемые):           */  Serial.print("Спутники: "); Serial.print(gps.satellites[GPS_ACTIVE]); Serial.print("/"); Serial.print(gps.satellites[GPS_VISIBLE]); Serial.print(". ");
     /* Геометрический фактор ухудшения точности:  */  Serial.print("PDOP: "); Serial.print(gps.PDOP); Serial.print(", HDOP: "); Serial.print(gps.HDOP); Serial.print(", VDOP: "); Serial.print(gps.VDOP); Serial.print(". ");
     /* Ошибка определения времени:                */  if(gps.errTim){Serial.print("Вемя недостоверно. "           );}
     /* Ошибка определения даты:                   */  if(gps.errDat){Serial.print("Дата недостоверна. "           );}
     /* Ошибка позиционирования:                   */  if(gps.errPos){Serial.print("Координаты недостоверны. "     );}
     /* Ошибка определения скорости и курса        */  if(gps.errCrs){Serial.print("Скорость и курс недостоверны. ");}
     /*                                            */  Serial.print("\r\n");
                                                   //
     for(uint8_t j=0; j<30; j++){                  //  Проходим по всем элементам массива «i».
        if( i[j][0] ){                             //  Если у спутника есть ID, то выводим информацию о нём:
            if(j<9){Serial.print(" ");}   Serial.print(j+1);         Serial.print(") "  );
            Serial.print("Спутник "    ); Serial.print(sa[i[j][2]]); Serial.print(" "   );
            Serial.print("ID: "        ); Serial.print(   i[j][0] ); Serial.print(". "  );
            Serial.print("Уровень: "   ); Serial.print(   i[j][1] ); Serial.print("дБ. ");
            Serial.print("Азимут: "    ); Serial.print(   i[j][4] ); Serial.print("°. " );
            Serial.print("Возвышение: "); Serial.print(   i[j][5] ); Serial.print("°. " );
            if(i[j][3]){Serial.print("Участвует в позиционировании.");}
            Serial.print("\r\n");                  //
        }                                          //
    }                                              //
}                                                  //
                                                   //
//   ПРИМЕЧАНИЕ:                                   //  ВАЖНО:
//   В скетче определён состав пакета NMEA         //  Уменьшение идентификаторов в составе пакета NMEA приводит
//   обращением к функции composition();           //  к уменьшению количества данных содержащихся в нём.
//   Выбранные идентификаторы позволяют получить   //
//   всю необходимою информацию.                   //