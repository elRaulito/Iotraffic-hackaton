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
# Hardware configuration
![connection](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/images/Traffic/trafficlight_bb.png?raw=true)

Bleeding resistors for leds should be evaluated according to your leds, usually RED LEDS have a smaller forward voltage therefore they need a higher resistor.
The optocoupler is used in order to supply the turn on current not from the GPIO of esp32 but from the 3.3V.
Any item/toy or device can be turned on with the relay if it has a switch (even if it does not), just place the relay in parallel with the switch.
The LiPo charger will help you to charge the battery without removing it from the board.
The LDO TPS75933 is a 3.3V regulator with a very small voltage drop and power consumption. In this way even if the battery is discharging the 3.3V will be constant.
The connections of this device are the following

| TPS75933 | Board |
|----------|-------|
| EN       | GND   |
| IN       | Battery+ from 1904|
|GND       | GND    |
|OUT |3.3V|
|FB/PG|NOT CONNECT|

Pay attention to the optocoupler polarity


# The software environment

Before using our Traffic light we should run an istance of Keepy and Streams-http-gateway.
The traffic light will fetch the information from Keepy and retrieve the information of the streets and accidents.
# Configuration

When the esp32-cam turns on it will generate its own wifi, the name is **TrafficLightIoTraffic** and the password is 12345678, after connecting open a browser and go to the page http://192.168.1.1 This page will appear:

![dashboard](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/IoTraffic/IoTraffic-dash/dash.JPG?raw=true)

Before pressing Activate traffic light let's configure all the parameters (**parameters are stored in Not volatile storage of ESP32 therefore you need to set everything only the first time**), 
like the wifi

![wifi](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/IoTraffic/IoTraffic-dash/wifi.JPG?raw=true)

the address and the port of Keepy

![keepy](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/IoTraffic/IoTraffic-dash/keepy.JPG?raw=true)

The channel from which read the data, in this way the traffic light will read only the data that are correctly written into the tangle

![detection](https://github.com/elRaulito/Iotraffic-hackaton/blob/main/IoTraffic/IoTraffic-dash/channel.JPG?raw=true)

Now just press **activate traffic light** in the homepage and your hub will start to change the timing according to the data in the channel.
