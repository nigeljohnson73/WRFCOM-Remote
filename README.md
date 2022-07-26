# WRFCOM Remote

This is a small project that uses EPS32 technology to control and visualise the [Water Rocket Flight Computer](https://github.com/nigeljohnson73/WRFCOM) data.

This project uses BLE currently but I am hoping to include ESPNOW so that the Feather S2 ESP chips can be used (and many more devices).

Things to install
-----------------
I will assume you are comfortable with the Arduino IDE.

You should install the latest ESP32 boards into your board manager if you haven't already (depending on which you 
want to use). Add these to your "Additional Boards Manager URLs" section in preferences.

```
https://dl.espressif.com/dl/package_esp32_index.json, 
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Code to update
--------------
To maintain the ability for you to modify the settings without causing a code thing if you're working on the code, 
you should create a file called `myConfig.h` and in it define any of the overridable settings in `App.h` for example:

```
#define _DEVICE_NAME_ "WRFCOM-F5A510"
```

The device ID is obtainable from the actual flight computer, and it is output to the serial port when you are building the device.


Hardware required
-----------------
Apart from the controller you will also need sensors and battery bits. For the sake of sanity I used off the shelf QWIIC
connecting sensor breakout boards. If you're good with soldering, you can certainly make things a lot smaller (and 
probably cheaper). The servo bits did need a little cutting and soldering, but this appeared to be well within my capability.

The following microcontrollers have been tested and in my preferred order:

* [ESP32-S3 Feather][ESP32-S3] Dual core, BLE, WiFi 4MB flash, 2MB PSRAM
* [ESP32-S3 Feather][ESP32-S3-NOPSRAM] Dual core, BLE, WiFi 8MB flash, No PSRAM
* [ESP32-S2 Feather][ESP32-S2] No Bluetooth, so looking for ESPNOW workyness

Other parts are required:

* [LIPO battery pack][LIPO-500MAH]
* [Sparkfun LED stick][QWIIC-LED-STICK]
* [OLED Feather Wing][QWIIC-OLED]
* [100mm QWIIC connector cable][QWIIC-CONNECTOR-100]

[ESP32-S3]: https://thepihut.com/products/adafruit-esp32-s3-feather-with-4mb-flash-2mb-psram-stemma-qt-qwiic
[ESP32-S3-NOPSRAM]: https://thepihut.com/products/adafruit-esp32-s3-feather-with-stemma-qt-qwiic-8mb-flash-no-psram
[ESP32-S2]: https://thepihut.com/products/adafruit-esp32-s2-feather-2-mb-psram-and-stemma-qt-qwiic
[LIPO-500MAH]: https://smile.amazon.co.uk/dp/B095W6742D
[QWIIC-LED-STICK]: https://coolcomponents.co.uk/products/qwiic-led-stick-apa102c
[QWIIC-OLED]: https://thepihut.com/products/adafruit-featherwing-oled-128x64-oled-add-on-for-feather?variant=39817250275523
[QWIIC-CONNECTOR-100]: https://thepihut.com/products/stemma-qt-qwiic-jst-sh-4-pin-cable-100mm-long

Libraries required
------------------
* SparkFun Qwiic LED Stick Library
* Adafruit LC709203F (for the onboard LiPO monitor)
* Adafruit_SH110x and Adafruit_GFX (for the OLED display)
