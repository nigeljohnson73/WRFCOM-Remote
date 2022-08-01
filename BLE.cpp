#include "BLE.h"
#include "BLE_SVC.h"

#include <BLEDevice.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("9ddf3d45-ea85-467a-9b23-34a9e4900400");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("9ddf3d45-ea85-467a-9b23-34a9e4900603");

static bool ble_doConnect = false;
static bool ble_connected = false;
static bool ble_doScan = true;
static BLEAdvertisedDevice* myDevice;

// Core
static BLERemoteCharacteristic* pBatteryCharacteristic;
static BLERemoteCharacteristic* pIpAddressCharacteristic;
static BLERemoteCharacteristic* pWifiModeCharacteristic;
static BLERemoteCharacteristic* pArmCharacteristic;
static BLERemoteCharacteristic* pLogCharacteristic;
double ble_val_battery = 0;
String ble_val_ip_address = "";//"999.999.999.999";
String ble_val_wifi_mode = "";//"XX";
bool ble_val_arm = false;
bool ble_val_log = false;

// Capability
static BLERemoteCharacteristic* pHasGpsCharacteristic;
static BLERemoteCharacteristic* pIsGpsLockedCharacteristic;
static BLERemoteCharacteristic* pHasBmsCharacteristic;
bool ble_val_has_bms = false;
bool ble_val_has_gps = false;
bool ble_val_is_gps_locked = false;
bool ble_val_has_rtc = false;
bool ble_val_has_emu = false;
bool ble_val_has_imu = false;
bool ble_val_has_srv = false;
bool ble_val_has_temp = false;
bool ble_val_has_pressure = false;
bool ble_val_has_acc = false;
bool ble_val_has_gyro = false;
bool ble_val_has_mag = false;

// GPS
int ble_val_siv = 0;
double ble_val_latitude = 0;
double ble_val_longitude = 0;
double ble_val_altitude = 0;


// EMU
double ble_val_temp = 0;
double ble_val_pressure = 0;

// IMU - Acc
static BLERemoteCharacteristic* pAccZCharacteristic;
double ble_val_acc_x = 0;
double ble_val_acc_y = 0;
double ble_val_acc_z = 0;

double ble_val_gyro_x = 0;
double ble_val_gyro_y = 0;
double ble_val_gyro_z = 0;

double ble_val_mag_x = 0;
double ble_val_mag_y = 0;
double ble_val_mag_z = 0;

void resetValues() {
  ble_val_ip_address = "";//"999.999.999.999";
  ble_val_wifi_mode = "";//"XX";
  ble_val_arm = false;
  ble_val_log = false;

  ble_val_has_bms = false;
  ble_val_has_gps = false;
  ble_val_is_gps_locked = false;
  ble_val_has_rtc = false;
  ble_val_has_imu = false;
  ble_val_has_emu = false;
  ble_val_has_srv = false;

  ble_val_has_temp = false;
  ble_val_has_pressure = false;
  ble_val_has_acc = false;
  ble_val_has_gyro = false;
  ble_val_has_mag = false;

  ble_val_siv = 0;
  ble_val_latitude = 0;
  ble_val_longitude = 0;
  ble_val_altitude = 0;

  ble_val_pressure = 0;
  ble_val_temp = 0;

  ble_val_acc_x = 0;
  ble_val_acc_y = 0;
  ble_val_acc_z = 0;

  ble_val_gyro_x = 0;
  ble_val_gyro_y = 0;
  ble_val_gyro_z = 0;

  ble_val_mag_x = 0;
  ble_val_mag_y = 0;
  ble_val_mag_z = 0;

}

TrBLE BLE;
TrBLE::TrBLE() {};

bool TrBLE::isConnected() {
  return ble_connected;
};

// Core
double TrBLE::getBattery() {
  return ble_val_battery;
}
String TrBLE::getIpAddress() {
  return ble_val_ip_address;
}
String TrBLE::getWifiMode() {
  return ble_val_wifi_mode;
}
bool TrBLE::isArmed() {
  return ble_val_arm;
}
bool TrBLE::isLogging() {
  return ble_val_log;
}
void TrBLE::setArmed(bool tf) {
  // TBD
  Serial.print("TrBLE::setArmed(");
  Serial.print(tf ? "TRUE" : "FALSE");
  Serial.print(")");
  Serial.println();

  pArmCharacteristic->writeValue(tf ? 1 : 0);
//  pArmCharacteristic->notify();
}
void TrBLE::setLogging(bool tf) {
  // TBD
  Serial.print("TrBLE::setLogging(");
  Serial.print(tf ? "TRUE" : "FALSE");
  Serial.print(")");
  Serial.println();

  pLogCharacteristic->writeValue(tf ? 1 : 0);
//  pLogCharacteristic->notify();
}





// Capability
bool TrBLE::hasBms() {
  return ble_val_has_bms;
}
bool TrBLE::hasGps() {
  return ble_val_has_gps;
}
bool TrBLE::isGpsLocked() {
  return hasGps() && ble_val_is_gps_locked;
}
bool TrBLE::hasEmu() {
  return ble_val_has_emu;
}
bool TrBLE::hasImu() {
  return ble_val_has_imu;
}
bool TrBLE::hasRtc() {
  return ble_val_has_rtc;
}
bool TrBLE::hasSrv() {
  return ble_val_has_srv;
}
bool TrBLE::hasTemp() {
  return hasEmu() && ble_val_has_temp;
}
bool TrBLE::hasPressure() {
  return hasEmu() && ble_val_has_pressure;
}
bool TrBLE::hasAcc() {
  return hasImu() && ble_val_has_acc;
}
bool TrBLE::hasGyro() {
  return hasImu() && ble_val_has_gyro;
}
bool TrBLE::hasMag() {
  return hasImu() && ble_val_has_mag;
}




// GPS
int TrBLE::getSiv() {
  return ble_val_siv;
}
double TrBLE::getLatitude() {
  return ble_val_latitude;
}
double TrBLE::getLongitude() {
  return ble_val_longitude;
}
double TrBLE::getAltitude() {
  return ble_val_altitude;
}





// EMU
double TrBLE::getTemp() {
  return ble_val_temp;
}
double TrBLE::getPressure() {
  return ble_val_pressure;
}

// IMU - ACC
double TrBLE::getAccX() {
  return ble_val_acc_x;
}
double TrBLE::getAccY() {
  return ble_val_acc_y;
}
double TrBLE::getAccZ() {
  return ble_val_acc_z;
}





// IMU - GYRO
double TrBLE::getGyroX() {
  return ble_val_gyro_x;
}
double TrBLE::getGyroY() {
  return ble_val_gyro_y;
}
double TrBLE::getGyroZ() {
  return ble_val_gyro_z;
}





// IMU - MAG
double TrBLE::getMagX() {
  return ble_val_mag_x;
}
double TrBLE::getMagY() {
  return ble_val_mag_y;
}
double TrBLE::getMagZ() {
  return ble_val_mag_z;
}

// Boolean
static void notifyCallbackBoolean(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  bool val = (*pData) ? true : false;
#if _DEBUG_
  String out;
#endif // _DEBUG_

  String in = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (false) {

  } else if (in == String(ARMED_CHARACTERISTIC_UUID)) {
    ble_val_arm = val;
#if _DEBUG_
    out = "       Armed: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else  if (in == String(LOGGING_CHARACTERISTIC_UUID)) {
    ble_val_log = val;
#if _DEBUG_
    out = "     Logging: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(RTCENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_rtc = val;
#if _DEBUG_
    out = " RTC Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(BMSENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_bms = val;
#if _DEBUG_
    out = " BMS Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(GPSENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_gps = val;
#if _DEBUG_
    out = " GPS Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(GPSLOCKED_CHARACTERISTIC_UUID)) {
    ble_val_is_gps_locked = val;
#if _DEBUG_
    out = "  GPS Locked: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(EMUENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_emu = val;
#if _DEBUG_
    out = " EMU Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(IMUENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_imu = val;
#if _DEBUG_
    out = " IMU Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(SRVENABLED_CHARACTERISTIC_UUID)) {
    ble_val_has_srv = val;
#if _DEBUG_
    out = " SRV Enabled: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(CTEMPERATURE_CHARACTERISTIC_UUID)) {
    ble_val_has_temp = val;
#if _DEBUG_
    out = "    Has Temp: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(CPRESSURE_CHARACTERISTIC_UUID)) {
    ble_val_has_pressure = val;
#if _DEBUG_
    out = "Has Pressure: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(CACC_CHARACTERISTIC_UUID)) {
    ble_val_has_acc = val;
#if _DEBUG_
    out = "     Has ACC: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(CGYRO_CHARACTERISTIC_UUID)) {
    ble_val_has_gyro = val;
#if _DEBUG_
    out = "    Has GYRO: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else if (in == String(CMAG_CHARACTERISTIC_UUID)) {
    ble_val_has_mag = val;
#if _DEBUG_
    out = "     Has MAG: "; out += (val ? "TRUE" : "FALSE");
#endif // _DEBUG_

  } else {
#if _DEBUG_
    out = "Unknown value";
#endif // _DEBUG_
  }

#if _DEBUG_
  Serial.print(in);
  Serial.print(" (");
  if (length < 10) Serial.print(" ");
  Serial.print(length);
  Serial.print(" byte");
  if (length == 1) Serial.print(" ");
  else             Serial.print("s");
  Serial.print("): ");
  Serial.print(out);
  Serial.println();
#endif // _DEBUG_
}

// Value
static void notifyCallbackValue(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  int8_t rval8;
  int32_t rval32;
  int val;
#if _DEBUG_
  String out;
#endif // _DEBUG_

  if (length == 1) {
    memcpy(&rval8, pData, sizeof(rval8));
    val = rval8;
  } else {
    memcpy(&rval32, pData, sizeof(rval32));
    val = rval32;
  }

  String in = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (false) {
  } else if (in == String(BATTERY_CHARACTERISTIC_UUID.toString().c_str())) {
    ble_val_battery = float(val);
#if _DEBUG_
    out = "     Battery: "; out += ble_val_battery; out += "%";
#endif // _DEBUG_

  } else if (in == String(ACCZ_CHARACTERISTIC_UUID)) {
    ble_val_acc_z = float(val) / 1000.0;
#if _DEBUG_
    out = "       Acc-Z: "; out += ble_val_acc_z; out += " m/s2";
#endif // _DEBUG_

  } else {
#if _DEBUG_
    out = "Unknown indicator";
#endif // _DEBUG_
  }


#if _DEBUG_
  Serial.print(in);
  Serial.print(" (");
  if (length < 10) Serial.print(" ");
  Serial.print(length);
  Serial.print(" byte");
  if (length == 1) Serial.print(" ");
  else             Serial.print("s");
  Serial.print("): ");
  Serial.print(out);
  Serial.println();
#endif // _DEBUG_
}

// String
static void notifyCallbackString(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  char rval[length + 1];
  memcpy(&rval, pData, length);
  rval[length] = 0x00;
#if _DEBUG_
  String out;
#endif // _DEBUG_

  String in = pBLERemoteCharacteristic->getUUID().toString().c_str();
  if (false) {
  } else if (in == String(IPADDR_CHARACTERISTIC_UUID)) {
    ble_val_ip_address = rval;
#if _DEBUG_
    out = "  IP Address: "; out += rval;
#endif // _DEBUG_
  } else if (in == String(WIFI_CHARACTERISTIC_UUID)) {
    ble_val_wifi_mode = rval;
#if _DEBUG_
    out = "   WiFi mode: "; out += rval;
#endif // _DEBUG_
  } else {
#if _DEBUG_
    out = "Unknown string: '"; out += rval; out += "'";
#endif // _DEBUG_
  }

#if _DEBUG_
  Serial.print(in);
  Serial.print(" (");
  if (length < 10) Serial.print(" ");
  Serial.print(length);
  Serial.print(" byte");
  if (length == 1) Serial.print(" ");
  else             Serial.print("s");
  Serial.print("): ");
  Serial.print(out);
  Serial.println();
#endif // _DEBUG_
}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
#if _DEBUG_ && _XDEBUG_
      Serial.println("BLEClient::onConnect()");
#endif // _DEBUG_ && _XDEBUG_
    }

    void onDisconnect(BLEClient* pclient) {
      ble_connected = false;
      resetValues();
#if _DEBUG_ && _XDEBUG_
      Serial.println("BLEClient::onDisconnect()");
#endif // _DEBUG_ && _XDEBUG_
    }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
  pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

  BLERemoteService* pRemoteService;

  // Core
  pRemoteService = pClient->getService(BLEUUID(WRFCOM_CORE_SERVICE_UUID));
  if (pRemoteService == nullptr) {
    Serial.print("Failed to CORE service");
    Serial.println();

    pBatteryCharacteristic = nullptr;
  } else {
    // BATTERY LEVEL
    pBatteryCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BATTERY_CHARACTERISTIC_UUID));
    if (pBatteryCharacteristic == nullptr) {
      Serial.print("Failed to find BATTERY characteristic");
      Serial.println();
    } else {
      if (pBatteryCharacteristic->canNotify())
        pBatteryCharacteristic->registerForNotify(notifyCallbackValue);
    }

    // IP ADDRESS
    pIpAddressCharacteristic = pRemoteService->getCharacteristic(BLEUUID(IPADDR_CHARACTERISTIC_UUID));
    if (pIpAddressCharacteristic == nullptr) {
      Serial.print("Failed to find IPADDR characteristic");
      Serial.println();
    } else {
      if (pIpAddressCharacteristic->canNotify())
        pIpAddressCharacteristic->registerForNotify(notifyCallbackString);
    }

    // WIFI MODE
    pWifiModeCharacteristic = pRemoteService->getCharacteristic(BLEUUID(WIFI_CHARACTERISTIC_UUID));
    if (pWifiModeCharacteristic == nullptr) {
      Serial.print("Failed to find IPADDR characteristic");
      Serial.println();
    } else {
      if (pWifiModeCharacteristic->canNotify())
        pWifiModeCharacteristic->registerForNotify(notifyCallbackString);
    }

    // ARM STATUS
    pArmCharacteristic = pRemoteService->getCharacteristic(BLEUUID(ARMED_CHARACTERISTIC_UUID));
    if (pArmCharacteristic == nullptr) {
      Serial.print("Failed to find ARMED characteristic");
      Serial.println();
    } else {
      if (pArmCharacteristic->canNotify())
        pArmCharacteristic->registerForNotify(notifyCallbackBoolean);
    }
    // LOG STATUS
    pLogCharacteristic = pRemoteService->getCharacteristic(BLEUUID(LOGGING_CHARACTERISTIC_UUID));
    if (pLogCharacteristic == nullptr) {
      Serial.print("Failed to find LOGGING characteristic");
      Serial.println();
    } else {
      if (pLogCharacteristic->canNotify())
        pLogCharacteristic->registerForNotify(notifyCallbackBoolean);
    }
  }







  // Capabilities
  pRemoteService = pClient->getService(BLEUUID(WRFCOM_CAPABILITY_SERVICE_UUID));
  if (pRemoteService == nullptr) {
    Serial.print("Failed to CORE service");
    Serial.println();

    pHasBmsCharacteristic = nullptr;
  } else {
    // Has BMS
    pHasBmsCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BMSENABLED_CHARACTERISTIC_UUID));
    if (pHasBmsCharacteristic == nullptr) {
      Serial.print("Failed to find BMSENABLED characteristic");
      Serial.println();
    } else {
      if (pHasBmsCharacteristic->canNotify())
        pHasBmsCharacteristic->registerForNotify(notifyCallbackBoolean);
    }

    // Has GPS
    pHasGpsCharacteristic = pRemoteService->getCharacteristic(BLEUUID(GPSENABLED_CHARACTERISTIC_UUID));
    if (pHasGpsCharacteristic == nullptr) {
      Serial.print("Failed to find GPSENABLED characteristic");
      Serial.println();
    } else {
      if (pHasGpsCharacteristic->canNotify())
        pHasGpsCharacteristic->registerForNotify(notifyCallbackBoolean);
    }

    // Is GPS Locked
    pIsGpsLockedCharacteristic = pRemoteService->getCharacteristic(BLEUUID(GPSLOCKED_CHARACTERISTIC_UUID));
    if (pIsGpsLockedCharacteristic == nullptr) {
      Serial.print("Failed to find GPSLOCKED characteristic");
      Serial.println();
    } else {
      if (pIsGpsLockedCharacteristic->canNotify())
        pIsGpsLockedCharacteristic->registerForNotify(notifyCallbackBoolean);
    }
  }






  // IMU - ACC
  pRemoteService = pClient->getService(BLEUUID(WRFCOM_IMU_SERVICE_UUID));
  if (pRemoteService == nullptr) {
    Serial.print("Failed to IMU service");
    Serial.println();

    pAccZCharacteristic = nullptr;
  } else {
    pAccZCharacteristic = pRemoteService->getCharacteristic(BLEUUID(ACCZ_CHARACTERISTIC_UUID));
    if (pAccZCharacteristic == nullptr) {
      Serial.print("Failed to find ACC-Z characteristic");
      Serial.println();
    } else {
      if (pAccZCharacteristic->canNotify())
        pAccZCharacteristic->registerForNotify(notifyCallbackValue);

    }
  }

  ble_connected = true;
  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      // We have found a device, let us now see if it contains the service we are looking for.

      if (advertisedDevice.haveName() && String(advertisedDevice.getName().c_str()) == String(_DEVICE_NAME_)) {
        ble_doConnect = true;
        ble_doScan = true;

        BLEDevice::getScan()->stop();
        if (myDevice) {
          Serial.print("Deleting old device");
          Serial.println();
          delete myDevice;
        }
        //        Serial.print("Connecting to new device device");
        //        Serial.println();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);

        String comma = "";
        Serial.print("BLE Device: ");

        Serial.print(comma);
        Serial.print("Addr: ");
        Serial.print(advertisedDevice.getAddress().toString().c_str());
        comma = ", ";

        if (advertisedDevice.haveName()) {
          Serial.print(comma);
          Serial.print("Name: ");
          Serial.print(advertisedDevice.getName().c_str());
          comma = ", ";
        }
        if (advertisedDevice.haveRSSI()) {
          Serial.print(comma);
          Serial.print("RSSI: ");
          Serial.print(advertisedDevice.getRSSI());
          comma = ", ";
        }
        if (advertisedDevice.haveTXPower()) {
          Serial.print(comma);
          Serial.print("TX: ");
          Serial.print(advertisedDevice.getTXPower());
          comma = ", ";
        }
        Serial.println();



      } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks


void TrBLE::begin() {
  BLEDevice::init("");
  BLEDevice::setMTU(517);
#if _DEBUG_
  Serial.print("BLE initialised: OK");
  Serial.println();
#endif
}

void lookForDevice() {
  static unsigned long last_scan = 0;
  unsigned long now = millis();

  if (last_scan > 0 && (now - last_scan) < 10000) {
    // Too soon since last scan
    return;
  }
  resetValues();
  IND.setScanning();

  static BLEScan* pBLEScan = nullptr;

  if (!pBLEScan) {
#if _DEBUG_
    Serial.println("BLE::lookForDevice(): Configuring new scanner");
#endif
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
  } else {
#if _DEBUG_
    Serial.println("BLE::lookForDevice(): restarting scanner");
#endif
    BLEDevice::getScan()->start(5, false);

  }
  last_scan = now;
#if _DEBUG_
  Serial.println("BLE::lookForDevice(): scan complete");
#endif
}

// This is the Arduino main loop function.
void TrBLE::loop() {
  //  static unsigned long last_out = 0;
  //  unsigned long now = millis();
  //  if (now - last_out >= 1000) {
  //    Serial.println("BLE::loop()");
  //    last_out = now;
  //  }

  // If the flag "ble_doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (ble_doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    ble_doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (ble_connected) {
    //    String newValue = "Time since boot: " + String(millis() / 1000);
    //    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    //    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  } else if (ble_doScan) {
    lookForDevice();
  }

} // End of loop
