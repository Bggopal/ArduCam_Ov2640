// Подключаем библиотеки
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <time.h>

// Указываем идентификатор и пароль от своей WiFi-сети
const char* ssid = "Nokia 6";
const char* password = "bbbbbbbb";

WiFiClientSecure client;

void uploadData(String content) {
  Serial.println("Dropbox connecting...");
  if (client.connect("content.dropboxapi.com", 443)) {
    Serial.println("Dropbox connected");

    // Сформировать имя файла по шаблону времени
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    char file_name[64] = {0};
    strftime(file_name, 64, "log-%Y-%m-%d-%H-%M-%S.txt", &timeinfo);
    Serial.print("Upload ");
    Serial.println(file_name);

    // Отправка запроса
    client.println("POST /2/files/upload HTTP/1.1");
    client.println("Host: content.dropboxapi.com");
    client.println("Authorization: Bearer 8IXfthdHTkkAAAAAAAAAAf-IIMjdzq9i0Q6zGFQfmD5ixCA9UAR_YemqPKQM9Vb1");
    char dropbox_args[255] = {0};
    sprintf(dropbox_args,
            "{\"path\": \"/%s\", \"mode\": \"overwrite\", \"autorename\": true, \"mute\": false}", file_name);
    client.print("Dropbox-API-Arg: ");
    client.println(dropbox_args);
    client.println("Content-Type: application/octet-stream");
    client.print("Content-Length: ");
    client.println(content.length());
    client.println();
    client.println(content);

    delay(5000);

    client.stop();
    Serial.println("Disconnect");
    Serial.println();
  } else {
    Serial.println("Error: cannot connect");
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);

  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Установка времени через SMTP
  Serial.print("Setting time using SNTP");
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);

  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println("done");

  // Загрузка данных #1
  uploadData(String("Data from ESP32 - this is a test 1"));

  delay(5000);

  // Загрузка данных #2
  uploadData(String("Data from ESP32 - this is a test 2"));

  delay(5000);

  // Загрузка данных #3
  uploadData(String("Data from ESP32 - this is a test 3"));
}

void loop() {}
