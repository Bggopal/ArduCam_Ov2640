// libraries
//#include <WiFi.h>
//#include <WiFiClientSecure.h>
#include <time.h>
#include <HardwareSerial.h>

static const int RXPin = 16, TXPin = 17; // UART pinout

HardwareSerial MOD(1); // The MODial connection to the Neoway device
// Specify the ID and password from your WiFi network
//const char* ssid = "Nokia 6";
//const char* password = "9746973398";

static String token = "sl.BDHC-3Ue2iitOcrsn7-g7nlsktEh2-wpX7ZLisWzXEZZ87dZqhfZ4x7bCn10eNdafAnQn1wvfPv92l5wg7ko15E7ioalbh8OwDf4a75GL2LqWAoCocvwvRc3W3VbuM91HgXm270A7vQ";


void uploadData(String content) {
  Serial.println("Dropbox connecting...");

  const char* myDomain = "content.dropboxapi.com";
  String getAll = "", getBody = "";

  //client_cp.setInsecure();

  //if (client_cp.connect("content.dropboxapi.com", 443)) {
  //Serial.println("Dropbox connected");

  // Generate a filename from a time pattern
  time_t now = time(nullptr);
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);

  char file_name[64] = {0};
  strftime(file_name, 64, "log-%Y-%m-%d-%H-%M-%S.txt", &timeinfo);
  Serial.print("Upload ");
  Serial.println(file_name);

  MOD.println("AT+HTTPSPARA =url," + String(myDomain)); //http setting the destination
  delay(100);
  response();

  MOD.print("AT+HTTPSPARA =port,443\r"); //Querying Network Registration Information
  delay(100);
  response();

  //The connection is set up successfully only after setting the destination address and port ID correctly.
  MOD.print("AT+HTTPSSETUP\r"); //http setting the destination
  delay(1000);
  response();
  Serial.println("Connection successful");

  MOD.print("AT+HTTPSACTION=99,500\r"); //Querying Network Registration Information
  delay(1000);
  response();

  // Sending a request
  MOD.println("POST /2/files/upload HTTP/1.1");
  MOD.println("Host: content.dropboxapi.com");
  MOD.println("Authorization: Bearer " + token);
  char dropbox_args[255] = {0};
  sprintf(dropbox_args, "{\"path\": \"/%s\", \"mode\": \"add\", \"autorename\": true, \"mute\": false}", file_name);
  Serial.println(dropbox_args);
  MOD.print("Dropbox-API-Arg: ");
  MOD.println(dropbox_args);
  MOD.println("Content-Type: application/octet-stream");
  MOD.print("Content-Length: ");
  MOD.println(content.length());
  MOD.println();
  MOD.println(content);
  //response();

  int waitTime = 10000;   // timeout 10 seconds
  long startTime = millis();
  boolean state = false;

  while ((startTime + waitTime) > millis()) {
    Serial.print(".");
    delay(100);
    while (MOD.available()) {
      char c = MOD.read();
      if (state == true) getBody += String(c);
      if (c == '\n') {
        if (getAll.length() == 0) state = true;
        getAll = "";
      } else if (c != '\r')
        getAll += String(c);
      startTime = millis();
    }
    if (getBody.length() > 0) break;
  }
  MOD.print("AT+HTTPCLOSE\r"); //close the http connection
  delay(2000);
  response();
  Serial.println(getBody);

  delay(5000);

  //client_cp.stop();
  //Serial.println("Disconnect");
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  delay(10);
  MOD.begin(115200, SERIAL_8N1, RXPin, TXPin );
  if (!MOD) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareMODial pin configuration, check config");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }
  Serial.print("Module interface succefully");

  neoway_intilize();

  // Setting the time via SMTP
  Serial.print("Setting time using SNTP");
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);

  /*while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    }*/

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

void loop()
{
}

void neoway_intilize()
{

  delay(3000);
  Serial.println("Starting now......");
  delay(5000);
  MOD.print("AT\r"); //status of module
  //MOD.write(0x0D);
  delay(100);
  response();
  MOD.print("ATI\r"); // Module information
  delay(100);
  response();
  MOD.print("AT+COPS=0,0\r"); // for automatic network registration
  delay(100);
  response();
  MOD.print("AT+COPS?\r");   // status query of the network registration
  delay(100);
  response();
  MOD.print("AT+SETVOLTE=1\r"); //Enable voLte
  delay(100);
  response();
  MOD.print("AT+SETVOLTE?\r"); // check voLte status (response 1 indicates volte enabled)
  delay(100);
  response();
  MOD.print("AT+NETDMSG\r"); //Querying Network Registration Information
  delay(100);
  response();
  MOD.print("AT^SYSINFO\r");
  /*Obtaining System Information
     response belike = ^SYSINFO: 2,3,0,9,1
     <CR><LF>^SYSINFO:<srv_status>,<srv_domain>,<roam_status>,<sys_mode>,<sim_state>[,[<reserve>],<sys_submode>] <CR><LF><CR><LF>OK<CR><LF>
     2 - service
     3 -  PS + CS
     0/1 - No Roaming/ Roaming
     9 - LTE mode
     1 - valid
  */
  delay(100);
  response();
  MOD.print("AT+BANDLOCK=?\r"); //Querying Network Registration Information
  delay(100);
  response();
  MOD.print("AT+CREG=1\r"); //Querying Network Registration Information
  delay(100);
  response();
  MOD.print("AT+CREG?\r"); //Querying Network Registration Information
  delay(100);
  response();  //Querying Network Registration Information response shuld be 1,1 (0,1 indicates imporper reg )
  MOD.print("AT+CREG=?\r"); //Querying Network Registration Information
  delay(100);
  response(); //AT+CREG=? +CREG: (0-2) indicates tat this is tat
  MOD.print("AT+XIIC=1\r");
  delay(100);
  response(); // establishing ppp connection
  MOD.print("AT+XIIC?\r");
  delay(100);
  response(); //AT+XIIC?  quering the ppp connection will output the ip
  MOD.print("AT+NETAPN?\r");
  delay(100);
  response(); //AT+NETAPN? // shows the apn details
  MOD.print("AT^SYSINFO\r");
  /*Obtaining System Information
     response belike = ^SYSINFO: 2,3,0,9,1
     <CR><LF>^SYSINFO:<srv_status>,<srv_domain>,<roam_status>,<sys_mode>,<sim_state>[,[<reserve>],<sys_submode>] <CR><LF><CR><LF>OK<CR><LF>
     2 - service
     3 -  PS + CS
     0/1 - No Roaming/ Roaming
     9 - LTE mode
     1 - valid
  */
  delay(100);
  response();
}

void response()
{
  if (MOD.available())
  {
    Serial.println(MOD.readString());
    //Serial.println(a);
  }
}
