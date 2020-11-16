## Autovelox

<img src="https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/Pictures/autovelox.jpg" width="400">
This is the source code of a speed limit detector, when an object gets closer to the camera with a speed higher than the treeshold the camera takes a picture.
The picture is then uploaded to the Tangle with the information regarding the speed, the limit speed and the fine to pay.

# Build an flash

After installing ESP-IDF 4.0 (**Some libraries don't work with 4.1 version**) and the esp-camera library (it is not already included in the components of esp-idf) go into the Autovelox folder open esp-idf and run the command
```
idf.py all
```

Now the project should be built and you are ready to flash it.
Connect the esp32-cam as follows with FTDI232
| ESP32-CAM GPIO| FTDI232 |
| ------------- | ------------- |
| 3V3           | VCC           |
| GND           | GND           |
| V0R           | TX           |
| V0T           | RX           |
| IO0 with GND   |  --          |

![connection](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/AI/programming.jpg?raw=true)

Press the reset button on esp32-cam and run the command 

```
idf.py flash

```
# The software environment

Before using our cam we should run an istance of Keepy, Streams-http-gateway.
The cam will send images to keepy encoded in base64, keepy will send the information to Streams-http-gateway therefore the data will appear on the channel of the Tangle.

# Running the program

To use the autovelox connect the ultrasonic sensor and the camera, to the power supply. We will use some resistances to protect the esp32-cam,
the ultrasonic sensor has a 5V voltage range while esp32 has 3.3V, we need to protect esp32 from the echo signal. Before connecting the FTDI232 change the output voltage to 5V.

| ESP32-CAM GPIO| FTDI232 |
| ------------- | ------------- |
| 5V          | VCC           |
| GND           | GND           |

| ESP32-CAM GPIO| HCSR-04 |
| ------------- | ------------- |
| 5V          | VCC           |
| GND           | GND           |
| IO2          | ECHO          |
| IO12           | TRIG           |

![](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/Autovelox/autovelox_bb.jpg)

Connect ftdi232 to an USB port and the autovelox will turn on.

# Configuration

When the esp32-cam turns on it will generate its own wifi, the name is **AutoveloxIoTraffic** and the password is 12345678, after connecting open a browser and go to the page http://192.168.1.1 This page will appear:

![dashboard](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/Autovelox/Autovelox-dash/dash.PNG?raw=true)

Before pressing Activate camera let's configure all the parameters (**parameters are stored in Not volatile storage of ESP32 therefore you need to set everything only the first time**), 
like the wifi

![wifi](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/Autovelox/Autovelox-dash/wifi.PNG?raw=true)

the address and the port of Keepy

![keepy](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/Autovelox/Autovelox-dash/keepy.PNG?raw=true)

the device ID in the whitelist in config.json of streams-http-gateway

![device](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/Autovelox/Autovelox-dash/device.PNG?raw=true)

The speed limit that I want to set as threeshold

![detection](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/Autovelox/Autovelox-dash/speed.PNG?raw=true)

Now just press **activate autovelox** in the homepage and your camera will start to pubblish pictures on the Tangle.
