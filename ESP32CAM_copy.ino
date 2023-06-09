#define CAMERA_MODEL_AI_THINKER

// Параметры камеры
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22


#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
//  #include <WebServer.h>
  #include <ESPAsyncWebServer.h>
#endif

//#include <ElegantOTA.h>
#include <AsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <GParser.h>
#include <esp_camera.h>
#include "SD_MMC.h"

const char* ssid = "EspG";
const char* password = "10203040";
// const int udpPort = 1234;               // Порт для прослушивания UDP

#if defined(ESP8266)
  ESP8266WebServer server(80);
#elif defined(ESP32)
  AsyncWebServer server(80);
  AsyncEventSource events("/events");
  AsyncWebSocket ws("/ws");
#endif

bool recording = false;

float Data[4] {0};

void handleRoot(AsyncWebServerRequest *request) {
  String html = "<html><head><meta charset=\"utf-8\"></head><body style=\"background-color: #cd7bff ;\">";
  html += "<h1 align=\"center\">\Инсектоспутники вперед! (Прошивка успешна. Великая победа!)</h1>";
  html += "<p align=\"center\">Температура в корпусе: " + String(Data[0]) + "</p>";
  html += "<p align=\"center\">Температура в колбе таракана: " + String(Data[1]) + "</p>";
  html += "<p align=\"center\">Давление: <span style=\"color: #ff0000;\">" + String(Data[2]) + "</span>" + "</p>";
  html += "<p align=\"center\">Высота относительно запуска: " + String(Data[3]) + "</p>";
  html += "</body></html>";
  request->send(200, "text/html", html);
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    for(int i = 3; i >= 0; i--){
      digitalWrite(4, i % 2);
      delay(100);
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  

  server.on("/", HTTP_GET, handleRoot);
  
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

}

void loop()
{
  char str[30] = {0};
//  AsyncElegantOTA.handle();
  if (WiFi.status() != WL_CONNECTED) 
  { // проверка статуса подключения
    Serial.println("WiFi connection lost"); // вывод сообщения об отключении
    for(int i = 3; i >= 0; i--)
    {
      digitalWrite(4, i % 2);
      delay(100);
    }
    ESP.restart();
  }
  // 30.00, 30.00, 300.00, 300,00
  if (Serial.available() > 0) {
    String bufString = Serial.readString();       // читаем как строку
    byte dividerIndex = bufString.indexOf(';');   // ищем индекс разделителя
    String buf_1 = bufString.substring(0, dividerIndex);    // создаём строку с первым числом
    byte dividerIndex2 = bufString.indexOf(';', dividerIndex + 1);   // ищем индекс разделителя
    String buf_2 = bufString.substring(dividerIndex + 1, dividerIndex2);   // создаём строку со вторым числом
    dividerIndex = bufString.indexOf(';', dividerIndex2 + 1);   // ищем индекс разделител
//    dividerIndex2 = bufString.indexOf(';', dividerIndex + 1);   // ищем индекс разделителя
    String buf_3 = bufString.substring(dividerIndex2 + 1, dividerIndex);    // создаём строку с первым числом
    dividerIndex = bufString.indexOf(';', dividerIndex2 + 1);   // ищем индекс разделител
    dividerIndex2 = bufString.indexOf(';', dividerIndex + 1);   // ищем индекс разделителя
    String buf_4 = bufString.substring(dividerIndex + 1);   // создаём строку со вторым числом
    Data[0] = buf_1.toFloat();  
    Data[1] = buf_2.toFloat(); 
    Data[2] = buf_3.toFloat();
    Data[3] = buf_4.toFloat();
    delay(0);
    if(Data[1] < 20) 
      Serial.print("$1 ");
    else
      Serial.print("$0 ");
    if((Data[3] > 200) || (millis() > 1000 * 600)) 
      Serial.println("1;");
    else
      Serial.println("0;");
  }
}
