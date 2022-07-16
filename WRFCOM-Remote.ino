#include <Wire.h>
#include "App.h"

void setup() {
  Wire.begin(); // Initialise the IIC bus
  Wire.setClock(400000); //Go super fast

  Serial.begin(115200);
  delay(5000);

  Serial.println("setup(): BLE Client");

  IND.begin();
  LCD.begin();
  BMS.begin();
  BLE.begin();
}

void loop() {
  BMS.loop();
  IND.loop();
  LCD.loop();
  BLE.loop();
}
