## IoTraffic

<img src="https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/Pictures/trafficlight.jpg" width="400">
This is the source code of IoTraffic, a traffic light that gets data about the streets from the tangle, according to this information it regulates the timing of the lights and in case of accident activates a relay.

# Build and flash
After installing ESP-IDF 4.0 (Some libraries don't work with 4.1 version) go into the IoTraffic folder open esp-idf and run the command
```
idf.py all
```

Now the project should be built and you are ready to flash it. Connect the esp32 to your device and run the command

```
idf.py flash 
```

# The software environment

Before using our Traffic light we should run an istance of Keepy and Streams-http-gateway.
The traffic light will fetch the information from Keepy and retrieve the information of the streets and accidents.
# Configuration

When the esp32-cam turns on it will generate its own wifi, the name is **TrafficLightIoTraffic** and the password is 12345678, after connecting open a browser and go to the page http://192.168.1.1 This page will appear:
