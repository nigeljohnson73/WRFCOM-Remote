#include "LCD.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
TrLCD LCD;

TrLCD::TrLCD() {};

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
  display.setCursor(0, 0);
  display.println("Welcome");
  display.display();

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

  if (!BLE.isConnected()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to WRFCOM");
    display.display();
    delay(15);
    return;
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connected to WRFCOM");
    //    display.display();
    delay(15);
  }

  if (!digitalRead(BUTTON_A)) display.print("A");
  if (!digitalRead(BUTTON_B)) display.print("B");
  if (!digitalRead(BUTTON_C)) display.print("C");
  delay(15);
  yield();
  display.display();
}
