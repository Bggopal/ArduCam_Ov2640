# ArduCam_Ov2640



## Hardware interface

<img src="https://user-images.githubusercontent.com/59637425/152762476-fb8aa79b-3e43-4060-84ff-a743bd3c004b.png" width="720" height="480">

<img src="https://user-images.githubusercontent.com/59637425/152762675-32fd2378-3c37-44e8-878f-4edbfe7517bb.png" width="400" height="400">

Using the jumper wires, connect the following pins on the ESP32 kit to the ArduCAM in the order they appear. Ensure that the ESP32 is not powered during this

|ESP32 |ArduCAM|Description        |
|:-----|:------|:------------------|
|Pin 5 | CS    |Chip Select for SPI|
|Pin 23| MOSI  |SPI MOSI           |
|Pin 19| MISO  |SPI MISO           |
|Pin 18| SCK   |SPI Clock          |
|Pin 21| SDA   |I2C Data           |
|Pin 22| SCL   |I2C Clock          |
|GND   | GND   |Common ground      |
|VCC   | VCC   |5V connection pin  | 

Note: VCC may be labelled '5V' on some ESP32 kits
## Software Setup

Firmware for the project was created using the Arduino IDE : USED IDE : Arduino IDE V1.8.12

Double check that the pin connections are correct, and then connect the ESP32 to your computer via the ESP32's micro-USB port. We will verify that the hardware is connected correctly when testing out the firmware
## Dependencies Required 

### Installing ESP32 package in Ardunio ide

To install the ESP32 board in your Arduino IDE, follow these next instructions:

1. In your Arduino IDE, go to File> Preferences

![image](https://user-images.githubusercontent.com/59637425/152768149-cdd5f29b-5f5a-4fb2-a584-919c507e360a.png)


2. Enter https://dl.espressif.com/dl/package_esp32_index.json into the “Additional Board Manager URLs” field as shown in the figure below. Then, click the “OK” button:

![image](https://user-images.githubusercontent.com/59637425/152768219-fd7e16cf-aa2b-4cc7-8f7d-5d12ad5e4da0.png)


**Note:** if you already have the ESP8266 boards URL, you can separate the URLs with a comma as follows:
```
https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
3. Open the Boards Manager. Go to Tools > Board > Boards Manager

![image](https://user-images.githubusercontent.com/59637425/152768338-d59061d2-7594-4960-878f-9bd09b57847c.png)

4. Search for ESP32 and press install button for the “ESP32 by Espressif Systems“:

![image](https://user-images.githubusercontent.com/59637425/152768376-e721104c-fc5a-42aa-a540-6cba06d31178.png)

Should be installed after a few seconds.

![image](https://user-images.githubusercontent.com/59637425/152768496-47519da2-dacc-4d52-b14e-49290e337d21.png)

## Getting Started ArduCAM Mini Camera modules with Arduino IDE

1.Copying libraries
The latest source code library and examples can be downloaded from the https://github.com/ArduCAM/Arduino.
The ArdCAM library is designed for Arduino platform,which is composed by four sub-libraries It is ArduCAM,ArduCAM_Touch,0V7670FIFO and UTFT4ArduCAM_SPI. These four libraries should be copied right under the libraries of Arduino directory in order to be recognized by the Arduino IDE. Also, the README.md is important,you should have a look.

![image](https://user-images.githubusercontent.com/59637425/152764493-ecba48b4-056b-4e6a-bab7-101f2d07f4de.png)

Configure the camera setting You need to modify the memorysaver.h file in order to enable OV2640_MINI_2MP or OV5642_MINI_5MP_PLUS or OV5640_MINI_5MP_PLUS camera modules.

![image](https://user-images.githubusercontent.com/59637425/152764675-3a129497-caed-4b6d-9b28-3ef9d0160717.png)


Need to install Arudcam_esp32_uno board (to use the example programs)

1. File-> Preferences -> (in " Additional Urls ") http://www.arducam.com/downloads/ESP32_UNO/package_ArduCAM_ESP32S_UNO_index.json (include the json data)-> Close

After adding URL

2. Tools -> Boards -> Board Manager -> (Search) ArduCam_Esp32_uno (install)

After Sucessful Installation select the select the ESP32_dev_module from the board manger and complie the program

[Reference 1](https://github.com/ArduCAM/Arduino)
[Reference 2](https://github.com/ArduCAM/ArduCAM_ESP32S_UNO)

**Now Arducam.h can be used**

## Pushing to drive

1. capture image to SPIFFS
2. Read image from SPIFFs
3. If image found in storage, convert it to base64 (header included)
4. Post to Drive

[Reference](https://github.com/fustyles/Arduino/tree/master/ESP32-CAM_SD_GoogleDrive)
