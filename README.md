# ArduCam_Ov2640

USED IDE : Arduino IDE V1.8.12

## Dependencies Required 

Need to install Arudcam_esp32_uno board

1. File-> Preferences -> (in " Additional Urls ") http://www.arducam.com/downloads/ESP32_UNO/package_ArduCAM_ESP32S_UNO_index.json (include the json data)-> Close

After adding URL

2. Tools -> Boards -> Board Manager -> (Search) ArduCam_Esp32_uno (install)

After Sucessful Installation select the select teh Arducam_esp32_uno board and complie the program

[Reference](https://github.com/ArduCAM/ArduCAM_ESP32S_UNO)

## Pushing to drive

1. capture image to SPIFFS
2. Read image from SPIFFs
3. If image found in storage, convert it to base64 (header included)
4. Post to Drive

[Reference](https://github.com/fustyles/Arduino/tree/master/ESP32-CAM_SD_GoogleDrive)
