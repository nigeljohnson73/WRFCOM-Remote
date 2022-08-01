#include "LCD.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
TrLCD LCD;

void drawHeader() {
  display.clearDisplay();
  display.setCursor(0, 0);

  // Draw the header (indicator and percentage, plus page number...)
  static unsigned long last_indicat_time = 0;
  static const unsigned long indicat_update = 125;
  unsigned long inow = millis();
  static char indicat[] = "|/-\\";
  static int last_indicat = -1;
  if ((inow - last_indicat_time) >= indicat_update) {
    last_indicat_time = inow;
    last_indicat += 1;
    if (last_indicat == strlen(indicat)) {
      last_indicat = 0;
    }
  }
  display.print(indicat[last_indicat]);
  display.print("                ");
  int pcnt = round(BMS.getCapacityPercent());
  if (pcnt < 100) display.print(" ");
  if (pcnt < 10) display.print(" ");
  display.print(pcnt);
  display.print("%");
  display.println();
  yield();
}

TrLCD::TrLCD() {}

void TrLCD::setScanning() {
  _current_page = 0;
  _buttons_enabled = false;
  //  display.clearDisplay();
  display.setCursor(0, 0);
  display.println();
  display.println();
  display.println();
  display.println(" Looking for buddy...");
  display.display();
  delay(15);
}

void TrLCD::buttonPressA() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonPressA()");
  Serial.println();
  if (_current_page == 0) {
    BLE.setArmed(!BLE.isArmed());
  }
}

void TrLCD::buttonPressB() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonPressB()");
  Serial.println();
  if (_current_page == 0) {
    BLE.setLogging(!BLE.isLogging());
  }
}

void TrLCD::buttonPressC() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonPressC()");
  Serial.println();
  _current_page += 1;
}

void TrLCD::buttonHoldA() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonHoldA()");
  Serial.println();
}

void TrLCD::buttonHoldB() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonHoldB()");
  Serial.println();
}

void TrLCD::buttonHoldC() {
  if (!_buttons_enabled) return;
  Serial.print("TrLCD::buttonHoldC()");
  Serial.println();
}




void TrLCD::begin() {
  if (!  display.begin(0x3C, true)) {
#if _DEBUG_
    Serial.print("LCD disconnected");
    Serial.println();
#endif
    return;
  }

  // Clear the buffer.
  display.clearDisplay();

  display.setRotation(1);
  display.setTextColor(SH110X_WHITE);
  //  display.setCursor(0, 0);
  //  display.println("Welcome");
  //  display.display();
  drawHeader();
  setScanning();

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

#if _DEBUG_
  Serial.print("LCD initialised: OK");
  Serial.println();
#endif
  _enabled = true;
}

void TrLCD::loop() {
  if (!isEnabled()) {
    return;
  }

  drawHeader();
  if (!BLE.isConnected()) {
    setScanning();
    return;
  } else {
    _buttons_enabled = true;

    if (_current_page == 0) {
      display.println();
      //    display.println(" Connected to WRFCOM");
      display.print("Name: ");
      display.print(BLE.getName());
      display.println();
      display.print("WiFi: ");
      display.print(BLE.getWifiMode());
      display.println();
      display.print("  IP: ");
      display.print(BLE.getIpAddress());
      display.println();
      if (BLE.hasBms()) {
        display.print("Batt: ");
        display.print(round(BLE.getBattery()), 0);
        display.print("%");
        display.println();
      } else {
        display.println();
      }
      display.println();
      display.print("  ");
      display.print(BLE.isArmed() ? "-ARMED-" : "UNARMED");
      display.print("    ");
      display.print(BLE.isLogging() ? "LOGGING" : "-IDLE-");
      display.println();
    } else     if (_current_page == 1) {
      display.println();
      display.println("PAGE 2 - coming soon");

    } else {
      _current_page = 0;
    }

  }
  yield();

  // Set to true, so the first time through here will set them as false and any press triggers them
  static bool last_a_state = true;
  static bool last_b_state = true;
  static bool last_c_state = true;

  static unsigned long last_a_millis;
  static unsigned long last_b_millis;
  static unsigned long last_c_millis;

  static unsigned long debounce_delay = 50;

  unsigned long now = millis();
  if (!digitalRead(BUTTON_A) != last_a_state) {
    last_a_millis = now;
  }
  if (!digitalRead(BUTTON_B) != last_b_state) {
    last_b_millis = now;
  }
  if (!digitalRead(BUTTON_C) != last_c_state) {
    last_c_millis = now;
  }

  if (now - last_a_millis > debounce_delay) {
    last_a_state = !last_a_state;
    if (last_a_state) {
      if (now - last_a_millis >= _button_hold_millis) {
        buttonHoldA();
      } else {
        buttonPressA();
      }
    }
  }

  if (now - last_b_millis > debounce_delay) {
    last_b_state = !last_b_state;
    if (last_b_state) {
      if (now - last_b_millis >= _button_hold_millis) {
        buttonHoldB();
      } else {
        buttonPressB();
      }
    }
  }

  if (now - last_c_millis > debounce_delay) {
    last_c_state = !last_c_state;
    if (last_c_state) {
      if (now - last_c_millis >= _button_hold_millis) {
        buttonHoldC();
      } else {
        buttonPressC();
      }
    }
  }

  delay(15);
  yield();
  display.display();
}
