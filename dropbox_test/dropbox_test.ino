// Подключаем библиотеки
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <time.h>

// Указываем идентификатор и пароль от своей WiFi-сети
const char* ssid = "Nokia 6";
const char* password = "bbbbbbbb";

static String token = "BBukG_iHpgqOlkd9ljhOzw793cAjNmy6vRytgXEwoQaq4qgP60gYAojdvDUKKtn8pekxM7mBC9Atj3WY_Oa4HJTjixQsDasIummiU_NLqP3rJxR35KoIgWg7wn2DyKJjWu2x9AM";
WiFiClientSecure client_cp;

//client_cp.setInsecure();

void uploadData(String content) {
  Serial.println("Dropbox connecting...");
  if (client_cp.connect("content.dropboxapi.com", 443)) {
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
    client_cp.println("POST /2/files/upload HTTP/1.1");
    client_cp.println("Host: content.dropboxapi.com");
    client_cp.println("Authorization: Bearer token");
    char dropbox_args[255] = {0};
    sprintf(dropbox_args,
            "{\"path\": \"/%s\", \"mode\": \"overwrite\", \"autorename\": true, \"mute\": false}", file_name);
    client_cp.print("Dropbox-API-Arg: ");
    client_cp.println(dropbox_args);
    client_cp.println("Content-Type: application/octet-stream");
    client_cp.print("Content-Length: ");
    client_cp.println(content.length());
    client_cp.println();
    client_cp.println(content);

    delay(5000);

    client_cp.stop();
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
