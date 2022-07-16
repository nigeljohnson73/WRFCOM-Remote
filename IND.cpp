#include "IND.h"

#include <Adafruit_NeoPixel.h>
#include <Qwiic_LED_Stick.h> // Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_LED_Stick

#if ESP32

#ifndef LED_PIN
#define LED_PIN 13
#endif

#ifndef NEO_PIN
#define NEO_PIN PIN_NEOPIXEL
#endif

#else // ESP32

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#define NEO_PIN 6
#endif

#endif // ESP32

#define NEO_BRIGHTNESS 10
#define LED_DELAY 10


#define STICK_BLE_PIN uint8_t(0)
#define STICK_GPS_PIN uint8_t(1)
#define STICK_BMS_PIN uint8_t(9)
#define STICK_RBMS_PIN uint8_t(8)

uint8_t c_red[] = {0xff, 0x00, 0x00};
uint8_t c_amber[] = {0xff, 0x66, 0x00};
uint8_t c_green[] = {0x00, 0xff, 0x00};
uint8_t c_blue[] = {0x00, 0x00, 0xff};
uint8_t c_purple[] = {0x00, 0x00, 0xff};

LED LEDStick;
Adafruit_NeoPixel NEO(1, NEO_PIN, NEO_GRB + NEO_KHZ800);
TrIND IND;

TrIND::TrIND(): _blip_duration(_blip_on_duration + _blip_off_duration)
{};

void TrIND::begin() {
  NEO.begin();
  NEO.clear();
  NEO.setPixelColor(0, NEO.Color(0x11, 0x00, 0x11));
  NEO.show();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Show there is some power while a scan goes on

  if (LEDStick.begin() == false) {
    Serial.println("Qwiic LED Stick failed to initialise!");
  } else {
    _has_stick = true;
  }

  if (_has_stick) {
    LEDStick.LEDOff();
    delay(LED_DELAY);
  }
#if _DEBUG_
  Serial.print("IND initialised: OK");
  Serial.println();
#endif
  _enabled = true;
}

void TrIND::setScanning() {
  _n_blips = 0;
  digitalWrite(LED_PIN, LOW); // Show there is some power while a scan goes on
  NEO.setPixelColor(0, NEO.Color( c_purple[0] / NEO_BRIGHTNESS, c_purple[1] / NEO_BRIGHTNESS, c_purple[2] / NEO_BRIGHTNESS));
  NEO.show();

  if (_has_stick) {
    LEDStick.setLEDBrightness(STICK_BLE_PIN, _stick_brightness);
    LEDStick.setLEDColor(STICK_BLE_PIN, c_purple[0], c_purple[1], c_purple[2]);
    for (int i = 1; i <= 8; i++) { // Switch off all the other LEDS except the battery one.
      LEDStick.setLEDBrightness(i, 0);
    }
    delay(LED_DELAY);
  }
}

void TrIND::loop() {
  if (!isEnabled()) {
    return;
  }

  // BLE connection
  if (BLE.isConnected()) {
    _blip = false;
    if (_has_stick) {
      LEDStick.setLEDBrightness(STICK_BLE_PIN, _stick_brightness);
      delay(LED_DELAY);
      LEDStick.setLEDColor(STICK_BLE_PIN, c_blue[0], c_blue[1], c_blue[2]);
      delay(LED_DELAY);

      // HERE: This is where we probe all the details and show the lights
      if (BLE.hasBms()) {
        LEDStick.setLEDBrightness(STICK_RBMS_PIN, _stick_brightness);
        delay(LED_DELAY);
        double pcnt = BLE.getBattery();
        if (pcnt <= 10) {
          LEDStick.setLEDColor(STICK_RBMS_PIN, c_red[0], c_red[1], c_red[2]);
          delay(LED_DELAY);
        } else if (pcnt <= 30) {
          LEDStick.setLEDColor(STICK_RBMS_PIN, c_amber[0], c_amber[1], c_amber[2]);
          delay(LED_DELAY);
        } else { // pcnt > 30
          LEDStick.setLEDColor(STICK_RBMS_PIN, c_green[0], c_green[1], c_green[2]);
          delay(LED_DELAY);
        }
      } else { // !BLE.hasBattery()
        LEDStick.setLEDBrightness(STICK_RBMS_PIN, 0);
        delay(LED_DELAY);
      }

      if (BLE.hasGps()) {
        LEDStick.setLEDBrightness(STICK_GPS_PIN, _stick_brightness);
        delay(LED_DELAY);
        if (BLE.isGpsLocked()) {
          LEDStick.setLEDColor(STICK_GPS_PIN, c_green[0], c_green[1], c_green[2]);
          delay(LED_DELAY);
        } else {
          int siv = BLE.getSiv();
          if (siv == 0) {
            LEDStick.setLEDColor(STICK_GPS_PIN, c_red[0], c_red[1], c_red[2]);
            delay(LED_DELAY);
          } else { // >=1 but < locked
            LEDStick.setLEDColor(STICK_GPS_PIN, c_amber[0], c_amber[1], c_amber[2]);
            delay(LED_DELAY);
          }
        }
      } else { // !BLE.hasGps()
        LEDStick.setLEDBrightness(STICK_GPS_PIN, 0);
        delay(LED_DELAY);
      }









    } else { // !_has_stick
      NEO.setPixelColor(0, NEO.Color(c_red[0] / NEO_BRIGHTNESS, c_red[1] / NEO_BRIGHTNESS, c_red[2] / NEO_BRIGHTNESS));
      NEO.show();
    }
  } else { // !BLE.connected()
    if (_has_stick) {
      LEDStick.setLEDBrightness(STICK_BLE_PIN, _stick_brightness);
      LEDStick.setLEDColor(STICK_BLE_PIN, c_red[0], c_red[1], c_red[2]);
      delay(LED_DELAY);
    } else { // !_has_stick
      NEO.setPixelColor(0, NEO.Color(c_red[0] / NEO_BRIGHTNESS, c_red[1] / NEO_BRIGHTNESS, c_red[2] / NEO_BRIGHTNESS));
      NEO.show();
    }
  }

  // Battery this side
  if (BMS.isEnabled()) {
    LEDStick.setLEDBrightness(STICK_BMS_PIN, _stick_brightness);
    delay(LED_DELAY);
    double pcnt = BMS.getCapacityPercent();
    static unsigned long last_out = 0;
    unsigned long now = millis();
    if ((now - last_out) >= 1000) {
      Serial.print("Battery: ");
      Serial.print(pcnt);
      Serial.print("%");
      Serial.println();
      last_out = now;
    }
    if (pcnt <= 10) {
      LEDStick.setLEDColor(STICK_BMS_PIN, c_red[0], c_red[1], c_red[2]);
      delay(LED_DELAY);
    } else if (pcnt <= 30) {
      LEDStick.setLEDColor(STICK_BMS_PIN, c_amber[0], c_amber[1], c_amber[2]);
      delay(LED_DELAY);
    } else { // pcnt > 30
      LEDStick.setLEDColor(STICK_BMS_PIN, c_green[0], c_green[1], c_green[2]);
      delay(LED_DELAY);
    }
  } else { // !BMS.isEnabled()
    LEDStick.setLEDBrightness(STICK_BMS_PIN, 0);
    delay(LED_DELAY);
  }

  // handle LED blipping
  unsigned long now = millis();
  if ((now - _sequence_started) < _sequence_duration) {
    // So we are in a sequence
    if (_n_blips > 0) {
      // we should be blipping
      if ((now - _blip_started) <= _blip_on_duration) {
        // In the honemoon period, light 'er up
        _blip = true;
      } else {
        // regardless of whether we should preparing another pone or just in the off period, still lights out
        _blip = false;
        if ((now - _blip_started) > (_blip_on_duration + _blip_off_duration)) {
          // we are past when the blip should finish, lets do another one... if needed
          if (_n_blips > 0) {
            _n_blips -= 1;
            _blip_started = now;
          }
        }
      }
    }
  } else   if (!BLE.isConnected()) {
    blink(2);
    _sequence_started = now;
  } else {
    _sequence_started = now;
  }
  digitalWrite(LED_PIN, _blip ? HIGH : LOW);
}
