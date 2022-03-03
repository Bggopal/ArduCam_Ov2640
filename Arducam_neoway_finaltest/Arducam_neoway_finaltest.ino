// ArduCAM demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with a supported camera modules, and can run on ArduCAM ESP32 UNO platform.
// This demo was made for Omnivision  2MP/5MP sensor.
// It will run the ArduCAM ESP32 2MP/5MP as a real 2MP/5MP digital camera, provide both JPEG capture.
// The demo sketch will do the following tasks:
// 1. Set the sensor to JPEG mode.
// 2. Capture and buffer the image to FIFO every 5 seconds
// 3. Store the image to Micro SD/TF card with JPEG format in sequential.
// 4. Resolution can be changed by myCAM.set_JPEG_size() function.
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM ESP8266 2MP/5MP shield
// and use Arduino IDE 1.8.1 compiler or above

#include <Wire.h>
//#include <WiFi.h>
//#include <WiFiClientSecure.h>
#include <SPI.h>
#include "FS.h"
#include "SPIFFS.h"
//#include "mbedtls/base64.h"
#include "Base64.h"
#include <ArduCAM.h>
#include "memorysaver.h"

#include <HardwareSerial.h>
//#include "neoway.h"

static const int RXPin = 16, TXPin = 17; // UART pinout

HardwareSerial MOD(1); // The MODial connection to the Neoway device

#if !(defined ESP32 )
#error Please select the ArduCAM ESP32 UNO board in the Tools/Board
#endif

//This demo can work on OV2640_MINI_2MP/OV5640_MINI_5MP_PLUS/OV5642_MINI_5MP_PLUS/OV5642_MINI_5MP_PLUS/
//OV5642_MINI_5MP_BIT_ROTATION_FIXED/ ARDUCAM_SHIELD_V2 platform.
#if !(defined (OV2640_MINI_2MP)||defined (OV2640_MINI_2MP_PLUS)||defined (OV5640_MINI_5MP_PLUS) || defined (OV5642_MINI_5MP_PLUS) \
    || defined (OV5642_MINI_5MP) || defined (OV5642_MINI_5MP_BIT_ROTATION_FIXED) \
    ||(defined (ARDUCAM_SHIELD_V2) && (defined (OV2640_CAM) || defined (OV5640_CAM) || defined (OV5642_CAM))))
#error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define FORMAT_SPIFFS_IF_FAILED true

const int CAM_POWER_ON = 10;
// set GPIO16 as the slave select :
const int CS = 5;
//Version 2,set GPIO0 as the slave select :
char pname[20];
//static int  index=0;
byte buf[256];
static int i = 0;
static int k = 0;
uint8_t temp = 0, temp_last = 0;
uint32_t length = 0;
bool is_header = false;

#if defined (OV2640_MINI_2MP)||defined (OV2640_MINI_2MP_PLUS) || defined (OV2640_CAM)
ArduCAM myCAM(OV2640, CS);
#elif defined (OV5640_MINI_5MP_PLUS) || defined (OV5640_CAM)
ArduCAM myCAM(OV5640, CS);
#elif defined (OV5642_MINI_5MP_PLUS) || defined (OV5642_MINI_5MP) || defined (OV5642_MINI_5MP_BIT_ROTATION_FIXED) ||(defined (OV5642_CAM))
ArduCAM myCAM(OV5642, CS);
#endif

/*const char* ssid = "Nokia 6";
  const char* password = "bbbbbbbb";*/

static String token = "sl.BDG68NhPtyu2kTD6J5A2C_bC0fRHwJbOwTtj7IqtiYwGrNytaWjmnHWNWA0tln_-zSF2PUaFoTWKMSy3nnPZVxDf0Q5syQHj-jtRgRXEvosASKP4zZYFschKIl5LfYXAtZhMk9zWWQs";

void capture2SD(fs::FS &fs, const char * path) {
  File file ;
  //Flush the FIFO
  myCAM.flush_fifo();
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();
  Serial.println(F("Star Capture"));
  while (!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
  Serial.println(F("Capture Done."));
  length = myCAM.read_fifo_length();
  Serial.print(F("The fifo length is :"));
  Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //8M
  {
    Serial.println(F("Over size."));
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
  }
  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  i = 0;
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buf[i++] = temp;  //save the last  0XD9
      //Write the remain bytes in the buffer
      myCAM.CS_HIGH();
      file.write(buf, i);
      //Close the file
      file.close();
      Serial.println(F("Image save OK."));
      is_header = false;
      i = 0;
    }
    if (is_header == true)
    {
      //Write image data to buffer if not full
      if (i < 256)
        buf[i++] = temp;
      else
      {
        //Write 256 bytes image data to file
        myCAM.CS_HIGH();
        file.write(buf, 256);
        i = 0;
        buf[i++] = temp;
        myCAM.CS_LOW();
        myCAM.set_fifo_burst();
      }
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buf[i++] = temp_last;
      buf[i++] = temp;
    }
  }
}
void setup() {
  uint8_t vid, pid;
  uint8_t temp;
  static int i = 0;
  //set the CS as an output:
  pinMode(CS, OUTPUT);
  pinMode(CAM_POWER_ON , OUTPUT);
  digitalWrite(CAM_POWER_ON, HIGH);
  Wire.begin();
  Serial.begin(115200);
  //wifi_connect();
  MOD.begin(115200, SERIAL_8N1, RXPin, TXPin );
  if (!MOD) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareMODial pin configuration, check config");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }

  neoway_intilize();

  Serial.println(F("ArduCAM Start!"));
  //initialize SPI:
  SPI.begin();
  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);
  while (1) {
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55) {
      Serial.println(F("SPI interface Error!"));
      delay(2);
      continue;
    }
    else
      break;
  }

  //Add support for SPIFFS
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("SPIFFS Mount Successful");
#if defined (OV2640_MINI_2MP)||defined (OV2640_MINI_2MP_PLUS) || defined (OV2640_CAM)
  //Check if the camera module type is OV2640
  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    Serial.println(F("Can't find OV2640 module!"));
  else
    Serial.println(F("OV2640 detected."));
#elif defined (OV5640_MINI_5MP_PLUS) || defined (OV5640_CAM)
  //Check if the camera module type is OV5640
  myCAM.wrSensorReg16_8(0xff, 0x01);
  myCAM.rdSensorReg16_8(OV5640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg16_8(OV5640_CHIPID_LOW, &pid);
  if ((vid != 0x56) || (pid != 0x40))
    Serial.println(F("Can't find OV5640 module!"));
  else
    Serial.println(F("OV5640 detected."));
#elif defined (OV5642_MINI_5MP_PLUS) || defined (OV5642_MINI_5MP) || defined (OV5642_MINI_5MP_BIT_ROTATION_FIXED) ||(defined (OV5642_CAM))
  //Check if the camera module type is OV5642
  myCAM.wrSensorReg16_8(0xff, 0x01);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
  if ((vid != 0x56) || (pid != 0x42)) {
    Serial.println(F("Can't find OV5642 module!"));
  }
  else
    Serial.println(F("OV5642 detected."));
#endif
  //Change to JPEG capture mode and initialize the OV2640 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)||defined (OV2640_MINI_2MP_PLUS) || defined (OV2640_CAM)
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
#elif defined (OV5640_MINI_5MP_PLUS) || defined (OV5640_CAM)
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5640_set_JPEG_size(OV5640_320x240);
#elif defined (OV5642_MINI_5MP_PLUS) || defined (OV5642_MINI_5MP) || defined (OV5642_MINI_5MP_BIT_ROTATION_FIXED) ||(defined (OV5642_CAM))
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
#endif
  delay(1000);
}
void loop() {
  sprintf((char*)pname, "/%05d.jpg", k);
  capture2SD(SPIFFS, pname);
  listDir(SPIFFS, "/", 0);
  readImage(SPIFFS, pname);
  SendCapturedImage2Dropbox(SPIFFS, pname);
  deleteFile(SPIFFS, pname);
  k++;
  delay(60000);

}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readImage(fs::FS &fs, const char * path) {
  Serial.printf("Reading Image: %s\r\n", path);
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.println("read succsseful");
    break;
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

String SendCapturedImage2Dropbox(fs::FS &fs, const char * path) {

  Serial.print("application to upload to cloud \n");


  Serial.printf("Reading Image: %s\r\n", path);
  File file = fs.open(path);
  uint8_t *fileinput;
  unsigned int fileSize = file.size();
  fileinput = (uint8_t*)malloc(fileSize + 1);
  file.read(fileinput, fileSize);
  fileinput[fileSize] = '\0';
  file.close();

  char *input = (char *)fileinput;
  String imageFile = "data:image/jpeg;base64,";
  char output[base64_enc_len(3)];
  for (int i = 0; i < fileSize; i++)
  {
    base64_encode(output, (input++), 3);
    if (i % 3 == 0) imageFile += urlencode(String(output));
    Serial.print(".");
  }

  Serial.print("Image length is ");
  Serial.println(imageFile.length());
  char image_file[255] = {0};
  int Index;
  for (Index = 0; Index < imageFile.length(); Index = Index + 100)
  {
    uploadData(imageFile.substring(Index, Index + 100));
  }
  
  Serial.println("send DOne New on way");  
}

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

  int waitTime = 20000;   // timeout 10 seconds
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
  Serial.println("Disconnect");
  Serial.println();
}

String urlencode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
      //encodedString+=code2;
    }
    yield();
  }
  return encodedString;
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
