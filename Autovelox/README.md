## Autovelox

<img src="https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/Pictures/autovelox.jpg" width="300">
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
