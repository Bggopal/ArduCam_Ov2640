#include <HardwareSerial.h>

static const int RXPin = 16, TXPin = 17; // UART pinout

HardwareSerial MOD(1); // The MODial connection to the Neoway device

static String token = "BBukG_iHpgqOlkd9ljhOzw793cAjNmy6vRytgXEwoQaq4qgP60gYAojdvDUKKtn8pekxM7mBC9Atj3WY_Oa4HJTjixQsDasIummiU_NLqP3rJxR35KoIgWg7wn2DyKJjWu2x9AM";

void setup() {
  Serial.begin(115200);
  MOD.begin(115200, SERIAL_8N1, RXPin, TXPin );
  if (!MOD) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareMODial pin configuration, check config");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }
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
   * response belike = ^SYSINFO: 2,3,0,9,1
   * <CR><LF>^SYSINFO:<srv_status>,<srv_domain>,<roam_status>,<sys_mode>,<sim_state>[,[<reserve>],<sys_submode>] <CR><LF><CR><LF>OK<CR><LF>
   * 2 - service
   * 3 -  PS + CS
   * 0/1 - No Roaming/ Roaming
   * 9 - LTE mode
   * 1 - valid
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
   * response belike = ^SYSINFO: 2,3,0,9,1
   * <CR><LF>^SYSINFO:<srv_status>,<srv_domain>,<roam_status>,<sys_mode>,<sim_state>[,[<reserve>],<sys_submode>] <CR><LF><CR><LF>OK<CR><LF>
   * 2 - service
   * 3 -  PS + CS
   * 0/1 - No Roaming/ Roaming
   * 9 - LTE mode
   * 1 - valid
   */
  delay(100);
  response();
}

void loop() 
{
  MOD.print("AT+HTTPPARA =url,script.google.com\r"); //http setting the destination
  delay(1000);
  response();

  //The connection is set up successfully only after setting the destination address and port ID correctly.

  MOD.print("AT+HTTPSETUP \r"); //Querying Network Registration Information
  delay(3000);
  response();
  
  MOD.print("AT+HTTPCLOSE \r"); //close the http connection 
  delay(5000);
  response();

  delay(10000);
}

void response()
{
  if (MOD.available())
  {
    Serial.println(MOD.readString());
    //Serial.println(a);
  }
}
