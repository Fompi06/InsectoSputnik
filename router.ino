#include <WiFi.h>

const char* ssid = "EspG";     // Имя точки доступа Wi-Fi
const char* password = "10203040";  // Пароль точки доступа Wi-Fi

void setup() {
  Serial.begin(115200);

  // Соединение с Wi-Fi точкой доступа
  WiFi.softAP(ssid, password);

  // Вывод информации о созданной точке доступа
  Serial.println();
  Serial.print("Создана точка доступа с именем: ");
  Serial.println(ssid);
  Serial.print("Пароль для подключения: ");
  Serial.println(password);

  // Отображение IP-адреса точки доступа
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP-адрес точки доступа: ");
  Serial.println(myIP);
}

void loop() {
  // Получение списка подключенных устройств
  int numDevices = WiFi.softAPgetStationNum();
  
  // Вывод количества подключенных устройств
  Serial.print("Количество подключенных устройств: ");
  Serial.println(numDevices);
  
  delay(500);  // Пауза между отображением количества устройств
}
