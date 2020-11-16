# IoTraffic-hackaton
![cover](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/cover.png?raw=true)
## Introduction
We have heard many times about AI, IoT and Smart cities. But what if we want to put into practice theory and connect AI to the city thanks to the Tangle. That's how IoTraffic was created.
IoTraffic is a smart city ecosystem based on tangle and artificial intelligence, the components of this ecosystem are:
* [AIcam](https://github.com/elRaulito/Iotraffic-hackaton/tree/main/AIcam), a board based on esp32 capable of detecting cars, pedestrians, accidents and more and pubblish the data on the tangle
* [SmartTrafficLight](https://github.com/elRaulito/Iotraffic-hackaton/tree/main/IoTraffic), a traffic light that changes its timing according to the data on the tangle and calls the ambulance in case of accident
* [Autovelox](https://github.com/elRaulito/Iotraffic-hackaton/tree/main/Autovelox), a speed detector that takes a picture of the car and post it on the tangle with the data of the speed and the amount of the fine

## Software requirements

* [esp-idf 4.0](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#step-2-get-esp-idf) to build and flash the projects into ESP32 (IMPORTANT, SOME LIBRARIES IN VERSION 4.1 are deprecated, you will not be able to buil the project with last version of esp-idf)
* [esp32-camera](https://github.com/espressif/esp32-camera) to drive the camera of ESP32-CAM for AIcam and Autovelox, just unzip the file and copy into the components directory of esp-idf
* [Streams-http-gateway](https://github.com/iot2tangle/Streams-http-gateway) To create channels to upload our data on the tangle
* [keepy](https://github.com/iot2tangle/Keepy) to speed up the communication to and from the channel, the traffic light will get only the last 5 messages present on the tangle
* ~~[Google Cloud vision](https://cloud.google.com/vision/docs/setup) The artificial intelligence that recognize the objects is based on Google Cloud vision, therefore you will need an api key~~ (No more needed thanks to MypersonalAI)
* [MyPersonalAI](https://github.com/elRaulito/Iotraffic-hackaton/tree/main/MyPersonalAI) A neural network working in node.js listening on port 3000 to detect objects in images that are sent to it. [Here](https://github.com/tensorflow/tfjs-models/blob/master/coco-ssd/src/classes.ts) you can find the list of the objects that can be detected

## Bill of materials

* 2x[ESP32-CAM](https://banggood.app.link/unDr7fFKfbb) We will use 2 modules, one will be used as AI object detector, the second as autovelox
* [FTDI232](https://it.aliexpress.com/item/33037470874.html) To flash the source code into esp32-cam
* [ESP32-devkitC](https://www.mouser.it/ProductDetail/Espressif-Systems/ESP32-DevKitC-S1?qs=%2Fha2pyFaduifTIKK1pCXCmH3HrYUnhPj%2F%2FRqdKjyW8ui2J7Dyx2Ttw%3D%3D) To create the traffic light driver
* [Traffic light](https://ebay.us/JGwnTe) or some leds to drive
* A LiPo battery of 3.7V 650mAh or more
* [75933](https://www.mouser.it/ProductDetail/Texas-Instruments/TPS75933KC?qs=6zVL%252ByCp0mp%2FfuY5MLXvjA%3D%3D) As LDO from Lipo battery to ESP32-devkitC 
* [LiPo charger](https://www.adafruit.com/product/1904) to charge the lipo battery from USB without disconnecting from the board.
* [HCSR-04](https://www.mouser.it/ProductDetail/OSEPP-Electronics/HC-SR04?qs=wNBL%252BABd93PqZEhuhHkuOw==&vip=1&gclid=Cj0KCQiAwMP9BRCzARIsAPWTJ_Gt9pzniKUyhvmVTh6iJucvYOdlv3uCE02es0e44awKTKKJIK6dOqsaArqEEALw_wcB) As ultrasonic sensor to detect speed of cars
* [3V RELAY](https://www.banggood.com/it/BESTEP-1-Channel-3_3V-Low-Level-Trigger-Relay-Module-Optocoupler-Isolation-Terminal-p-1355736.html?utm_source=googleshopping&utm_medium=cpc_organic&gmcCountry=IT&utm_content=minha&utm_campaign=minha-it-it-pc&currency=EUR&createTmp=1&cur_warehouse=CN) A relay to turn on the ambulance
* If you dont have an ambulance just use anything you would like to turn on in case of accident detection
