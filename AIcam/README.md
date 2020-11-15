## AIcam

This project is a camera detection object based on AI, the object detected are sent to a channel on the Tangle.

# Build an flash

After installing ESP-IDF 4.0 (**Some libraries don't work with 4.1 version**) and the esp-camera library (it is not already included in the components of esp-idf) go into the AIcam folder open esp-idf and run the command
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

