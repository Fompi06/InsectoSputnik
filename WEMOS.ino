// Старый файл, на данный момент не актуален и не будет работать

#include <iarduino_Pressure_BMP.h>
#include <microDS18B20.h>
#include <GParser.h>
#define DS_PIN 0 // пин для термометров
#define HEAT_P D5
#define END_0 D1
#define END_1 D2
// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = {0x28, 0xFF, 0x64, 0x1, 0xB9, 0xFC, 0x77, 0x54};
uint8_t s2_addr[] = {0x28, 0x60, 0x43, 0x49, 0xF6, 0x96, 0x3C, 0x21};
MicroDS18B20<DS_PIN, s1_addr> senAir;  // Создаем термометр с адресацией
MicroDS18B20<DS_PIN, s2_addr> senIns;  // Создаем термометр с адресацией
iarduino_Pressure_BMP BMP;

uint64_t Timer;
bool Flag = 1;

void setup() {
  Serial.begin(115200);
  delay(1000);
  BMP.begin();  
}
void loop() {
  // асинхронное чтение нескольких датчиков смотри в примере async_read_many
  senAir.requestTemp();      // Запрашиваем преобразование температуры
  senIns.requestTemp();
  BMP.read();
  delay(1000);           // ожидаем результат 
  
  if (senAir.readTemp()) 
  {
    Serial.print(senAir.getTemp());
    Serial.print(", ");
  } else Serial.print("0, ");
  if (senIns.readTemp()) 
  {
    Serial.print(senIns.getTemp());
    Serial.print(", ");
  } else Serial.print("0, ");
  Serial.print(BMP.pressure);
  Serial.print(", ");
  Serial.println(BMP.altitude + 100);
  if(Serial.available() > 1)
  {
    char str[30];
    int amount = Serial.readBytesUntil(',', str, 30);
    str[amount] = NULL;
    GParser data(str, ',');
    int am = data.split();
    if(data[0]) 
    {
      digitalWrite(HEAT_P, 1);
    }
    if(data[1])
    {
      if  (Flag) Timer = millis();
      Flag = 0;
      digitalWrite(END_0, 1);
      if(millis() - Timer > 2000) 
      {
        digitalWrite(END_1, 1);
      }
    }
  }
}
