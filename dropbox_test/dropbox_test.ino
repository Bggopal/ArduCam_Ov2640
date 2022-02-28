// libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

// Specify the ID and password from your WiFi network
const char* ssid = "Nokia 6";
const char* password = "bbbbbbbb";

static String token = "sl.BCtWHA9L8l71jeVq8mnlpvgeGeK3NVtNSoKW6qzjShnWl0c1_cLdTWJ9ZhcILcw071YJRo7PcNhQzluylKCTwUScPSWdx9TGg2DNIJ5AEOYdpKtRQg5n8MHxXjQHxMHnM_8aBmvW8TQ";
WiFiClientSecure client_cp;

//client_cp.setInsecure();

void uploadData(String content) {
  Serial.println("Dropbox connecting...");

  String getAll="", getBody = "";

  client_cp.setInsecure();
  
  if (client_cp.connect("content.dropboxapi.com", 443)) {
    Serial.println("Dropbox connected");

    // Generate a filename from a time pattern
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    char file_name[64] = {0};
    strftime(file_name, 64, "log-%Y-%m-%d-%H-%M-%S.txt", &timeinfo);
    Serial.print("Upload ");
    Serial.println(file_name);

    // Sending a request
    client_cp.println("POST /2/files/upload HTTP/1.1");
    client_cp.println("Host: content.dropboxapi.com");
    client_cp.println("Authorization: Bearer "+token);
    char dropbox_args[255] = {0};
    //"path": "/Test","mode": "add","autorename": true,"mute": false
    sprintf(dropbox_args, "{\"path\": \"/%s\", \"mode\": \"add\", \"autorename\": true, \"mute\": false}", file_name);
    Serial.println(dropbox_args);
    client_cp.print("Dropbox-API-Arg: ");
    client_cp.println(dropbox_args);
    client_cp.println("Content-Type: application/octet-stream");
    client_cp.print("Content-Length: ");
    client_cp.println(content.length());
    client_cp.println();
    client_cp.println(content);

    int waitTime = 10000;   // timeout 10 seconds
    long startTime = millis();
    boolean state = false;
    
    while ((startTime + waitTime) > millis()) {
      Serial.print(".");
      delay(100);      
      while (client_cp.available()) {
          char c = client_cp.read();
          if (state==true) getBody += String(c);        
          if (c == '\n') {
            if (getAll.length()==0) state=true; 
            getAll = "";
          } else if (c != '\r')
            getAll += String(c);
          startTime = millis();
       }
       if (getBody.length()>0) break;
    }
    client_cp.stop();
    Serial.println(getBody);

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

  // Setting the time via SMTP
  Serial.print("Setting time using SNTP");
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);

  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println("done");

  // Data upload #1
  uploadData(String("Data from ESP32 - this is a test 1"));

  delay(5000);

  // Data upload #2
  uploadData(String("Data from ESP32 - this is a test 2"));

  delay(5000);

  // Data upload #3
  uploadData(String("Data from ESP32 - this is a test 3"));
}

void loop() {}
