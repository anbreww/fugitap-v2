# Fugitap V2 display manager

Improved version of the Fûgidaire kegerator tap display. Ported from ESP8266 to the ESP32.

## Features

* Supply voltage and current monitoring with INA219
* 2x 12V computer fans with hardware PWM control and RPM reading
* Flow meter input to measure beer flow
* LED output to indicate beer pours
* 2.4in TFT display (ILI9341) to display beer stats and fill level
* Hardware PWM control of LCD backlight
* 1-Wire temperature sensors to monitor keg or kegerator temperature
* WS2812 / Neopixel LED strings with 3.3V to 5V level conversion

The device stores beer name, style, abv and other informations which are sent
over MQTT. These are displayed on the small TFT screen. Beer level in the keg
and other information can be transmitted back over MQTT.


## Getting Started

Copy the `settings_template.h` file to `settings.h` and enter your own user
settings. The file is set to be ignored by git so you don't accidentally commit
your WiFi or MQTT credentials to github :)

OTA updates are enabled. After the first build, simply add the following line
to the `platformio.ini` file to enable remote updates :

```
upload_port = 192.168.0.17
```

(relace by the device's IP, obviously. It will display briefly during boot up)

If you want to upload to several boards without changing the configuration every
time, you can also call the following command from a terminal, or add it as a
build task to your editor :

```
pio run -t upload --upload-port 192.168.0.26
```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Andrew Watson** - *Initial work* - https://github.com/anbreww


## License

MIT License

## Acknowledgments