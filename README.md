# IoTraffic-hackaton
![alt text](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/cover.png?raw=true)
## Introduction
We have heard many times about AI, IoT and Smart cities. But what if we want to put into practice theory and connect AI to the city thanks to the Tangle. That's how IoTraffic was created.
IoTraffic is a smart city ecosystem based on tangle and artificial intelligence, the components of this ecosystem are:
* AIcam, a board capable of detecting cars, pedestrians, accidents and more and pubblish the data on the tangle
* SmartTrafficLight, a traffic light that changes its timing according to the data on the tangle
* Autovelox, a speed detector that takes a picture of the car and post it on the tangle

## Software requirements

* [esp-idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#step-2-get-esp-idf) to build and flash the projects into ESP32
* [esp32-camera](https://github.com/espressif/esp32-camera) to drive the camera of ESP32-CAM for AIcam and Autovelox
* [Streams-http-gateway](https://github.com/iot2tangle/Streams-http-gateway) To create channels to upload our data
* [keepy](https://github.com/iot2tangle/Keepy) to speed up the communication to and from the channel
* [Google Cloud vision](https://cloud.google.com/vision/docs/setup) The artificial intelligence that recognize the objects is based on Google Cloud vision, therefore you will need an api key

## Bill of materials

* 2x[ESP32-CAM](https://banggood.app.link/unDr7fFKfbb) We will use 2 modules, one will be used as AI object detector, the second as autovelox
* [FTDI232](https://it.aliexpress.com/item/33037470874.html) To flash the source code into esp32-cam
* [ESP32-devkitC](https://www.mouser.it/ProductDetail/Espressif-Systems/ESP32-DevKitC-S1?qs=%2Fha2pyFaduifTIKK1pCXCmH3HrYUnhPj%2F%2FRqdKjyW8ui2J7Dyx2Ttw%3D%3D) To create the traffic light driver
