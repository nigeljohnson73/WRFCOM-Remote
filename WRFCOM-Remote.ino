#include <Wire.h>
#include "App.h"

void setup() {
  Wire.begin(); // Initialise the IIC bus
  Wire.setClock(400000); //Go super fast

  Serial.begin(115200);
  delay(5000);

#if _DEBUG_
  Serial.print("WRFCOM-Remote ");
  Serial.print(VERSION);
  Serial.print(": ");
  Serial.print(__DATE__);
  Serial.print(" @ ");
  Serial.print(__TIME__);
  Serial.println();
#endif
  Serial.println("setup(): BLE Client");

  BMS.begin();
  NOW.begin();
  BLE.begin();
  IND.begin();
  LCD.begin();
}

void loop() {
  NOW.loop();
  BLE.loop();
  BMS.loop();
  IND.loop();
  LCD.loop();
}
