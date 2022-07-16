#include "BLE.h"
#include "BLE_SVC.h"

#include <BLEDevice.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("9ddf3d45-ea85-467a-9b23-34a9e4900400");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("9ddf3d45-ea85-467a-9b23-34a9e4900603");

static bool doConnect = false;
static bool connected = false;
static bool doScan = true;
static BLEAdvertisedDevice* myDevice;

// Core
static BLERemoteCharacteristic* pBatteryCharacteristic;
static BLERemoteCharacteristic* pIpAddressCharacteristic;
static BLERemoteCharacteristic* pWifiModeCharacteristic;
static BLERemoteCharacteristic* pArmCharacteristic;
static BLERemoteCharacteristic* pLogCharacteristic;
double val_battery = 0;
String val_ip_address = "0.0.0.0";
String val_wifi_mode = "XX";
bool val_arm = true;
bool val_log = false;

// Capability
static BLERemoteCharacteristic* pHasBmsCharacteristic;
bool val_has_bms = false;
bool val_has_gps = false;
bool val_is_gps_locked = false;
bool val_has_rtc = false;
bool val_has_emu = false;
bool val_has_imu = false;
bool val_has_srv = false;
bool val_has_temp = false;
bool val_has_pressure = false;
bool val_has_acc = false;
bool val_has_gyro = false;
bool val_has_mag = false;

// GPS
int val_siv = 0;
double val_latitude = 0;
double val_longitude = 0;
double val_altitude = 0;


// EMU
double val_temp = 0;
double val_pressure = 0;

// IMU - Acc
static BLERemoteCharacteristic* pAccZCharacteristic;
double val_acc_x = 0;
double val_acc_y = 0;
double val_acc_z = 0;

double val_gyro_x = 0;
double val_gyro_y = 0;
double val_gyro_z = 0;

double val_mag_x = 0;
double val_mag_y = 0;
double val_mag_z = 0;

void resetValues() {
  val_ip_address = "0.0.0.0";
  val_wifi_mode = "XX";
  val_arm = false;
  val_log = false;

  val_has_bms = false;
  val_has_gps = false;
  val_is_gps_locked = false;
  val_has_rtc = false;
  val_has_imu = false;
  val_has_emu = false;
  val_has_srv = false;

  val_has_temp = false;
  val_has_pressure = false;
  val_has_acc = false;
  val_has_gyro = false;
  val_has_mag = false;

  val_siv = 0;
  val_latitude = 0;
  val_longitude = 0;
  val_altitude = 0;

  val_pressure = 0;
  val_temp = 0;

  val_acc_x = 0;
  val_acc_y = 0;
  val_acc_z = 0;

  val_gyro_x = 0;
  val_gyro_y = 0;
  val_gyro_z = 0;

  val_mag_x = 0;
  val_mag_y = 0;
  val_mag_z = 0;

}

TrBLE BLE;
TrBLE::TrBLE() {};

bool TrBLE::isConnected() {
  return connected | doConnect;
};

// Core
double TrBLE::getBattery() {
  return val_battery;
}
String TrBLE::getIpAddress() {
  return val_ip_address;
}
String TrBLE::getWifiMode() {
  return val_ip_address;
}
bool TrBLE::isArmed() {
  return val_arm;
}
bool TrBLE::isLogging() {
  return val_log;
}
void TrBLE::setArmed(bool tf) {
  // TBD
}
void TrBLE::setLogging(bool tf) {
  // TBD
}





// Capability
bool TrBLE::hasBms() {
  return val_has_bms;
}
bool TrBLE::hasGps() {
  return val_has_gps;
}
bool TrBLE::isGpsLocked() {
  return hasGps() && val_is_gps_locked;
}
bool TrBLE::hasEmu() {
  return val_has_emu;
}
bool TrBLE::hasImu() {
  return val_has_imu;
}
bool TrBLE::hasRtc() {
  return val_has_rtc;
}
bool TrBLE::hasSrv() {
  return val_has_srv;
}
bool TrBLE::hasTemp() {
  return hasEmu() && val_has_temp;
}
bool TrBLE::hasPressure() {
  return hasEmu() && val_has_pressure;
}
bool TrBLE::hasAcc() {
  return hasImu() && val_has_acc;
}
bool TrBLE::hasGyro() {
  return hasImu() && val_has_gyro;
}
bool TrBLE::hasMag() {
  return hasImu() && val_has_mag;
}




// GPS
int TrBLE::getSiv() {
  return val_siv;
}
double TrBLE::getLatitude() {
  return val_latitude;
}
double TrBLE::getLongitude() {
  return val_longitude;
}
double TrBLE::getAltitude() {
  return val_altitude;
}





// EMU
double TrBLE::getTemp() {
  return val_temp;
}
double TrBLE::getPressure() {
  return val_pressure;
}

// IMU - ACC
double TrBLE::getAccX() {
  return val_acc_x;
}
double TrBLE::getAccY() {
  return val_acc_y;
}
double TrBLE::getAccZ() {
  return val_acc_z;
}





// IMU - GYRO
double TrBLE::getGyroX() {
  return val_gyro_x;
}
double TrBLE::getGyroY() {
  return val_gyro_y;
}
double TrBLE::getGyroZ() {
  return val_gyro_z;
}





// IMU - MAG
double TrBLE::getMagX() {
  return val_mag_x;
}
double TrBLE::getMagY() {
  return val_mag_y;
}
double TrBLE::getMagZ() {
  return val_mag_z;
}

// Core
static void notifyCallbackValue(  BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  int8_t rval8;
  int32_t rval32;
  int val;
  String unit = "";

  if (length == 1) {
    memcpy(&rval8, pData, sizeof(rval8));
    val = rval8;
  } else {
    memcpy(&rval32, pData, sizeof(rval32));
    val = rval32;
  }

  String in = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (in == String(BATTERY_CHARACTERISTIC_UUID.toString().c_str())) {
    val_battery = float(val);
    unit = "%";

  } else if (in == String(ACCZ_CHARACTERISTIC_UUID)) {
    val_acc_z = float(val) / 1000.0;
    unit = "%";
  } else {
    Serial.println("    Unknown characteristic");
  }


#if _DEBUG_
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" (");
  Serial.print(length);
  Serial.print(" bytes): ");
  Serial.print(val);
  Serial.print(" ");
  Serial.print(unit);
  Serial.println();
#endif // _DEBUG_
}

// Capability
static void notifyCallbackCapability(  BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  bool val = (*pData) ? true : false;

#if _DEBUG_
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" (");
  Serial.print(length);
  Serial.print(" bytes): ");
  Serial.print(val ? "TRUE" : "FALSE");
  Serial.println();
#endif // _DEBUG_

  String in = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (in == String(BMSENABLED_CHARACTERISTIC_UUID)) {
    val_has_bms = val;
  } else {
    Serial.println("    Unknown characteristic");
  }

}

//static void notifyCallbackAccZ(  BLERemoteCharacteristic* pBLERemoteCharacteristic,
//                                 uint8_t* pData,
//                                 size_t length,
//                                 bool isNotify) {
//  int32_t rval;
//  memcpy(&rval, pData, sizeof(rval));
//
//  val_acc_z = float(rval) / float(pow(10, 3));
//
//#if _DEBUG_
//  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//  Serial.print(" (");
//  Serial.print(length);
//  Serial.print(" bytes): ");
//  Serial.print(val_acc_z);
//  Serial.print(" m/s2");
//  Serial.println();
//#endif // _DEBUG_
//}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      Serial.println("BLEClient::onConnect()");
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      resetValues();
      Serial.println("BLEClient::onDisconnect()");
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
  }

  // Capabilities
  pRemoteService = pClient->getService(BLEUUID(WRFCOM_CAPABILITY_SERVICE_UUID));
  if (pRemoteService == nullptr) {
    Serial.print("Failed to CORE service");
    Serial.println();

    pHasBmsCharacteristic = nullptr;
  } else {
    pHasBmsCharacteristic = pRemoteService->getCharacteristic(BLEUUID(BMSENABLED_CHARACTERISTIC_UUID));
    if (pHasBmsCharacteristic == nullptr) {
      Serial.print("Failed to find HAS BATTERY characteristic");
      Serial.println();
    } else {
      if (pHasBmsCharacteristic->canNotify())
        pHasBmsCharacteristic->registerForNotify(notifyCallbackCapability);

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

  connected = true;
  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      // We have found a device, let us now see if it contains the service we are looking for.

      if (advertisedDevice.haveName() && String(advertisedDevice.getName().c_str()) == String(_DEVICE_NAME_)) {
        doConnect = true;
        doScan = true;

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
  Serial.println("setup() complete");
} // End of setup.

void lookForDevice() {
  static unsigned long last_scan = 0;
  unsigned long now = millis();

  if (last_scan > 0 && (now - last_scan) < 10000) {
    //    static unsigned long last_out = 0;
    //    unsigned long now = millis();
    //    if (now - last_out >= 500) {
    //      Serial.println("BLE::lookForDevice(): too soon");
    //      last_out = now;
    //    }
    return;
  }
  resetValues();

  IND.setScanning();
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  static BLEScan* pBLEScan = nullptr;

  if (!pBLEScan) {
    Serial.println("BLE::lookForDevice(): Configuring scanner");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
  } else {
    Serial.println("BLE::lookForDevice(): restarting scan scan");
    BLEDevice::getScan()->start(5, false);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino

  }
  last_scan = now;
  //  doScan = false;
  Serial.println("BLE::lookForDevice(): scan complete");
}

// This is the Arduino main loop function.
void TrBLE::loop() {
  //  static unsigned long last_out = 0;
  //  unsigned long now = millis();
  //  if (now - last_out >= 1000) {
  //    Serial.println("BLE::loop()");
  //    last_out = now;
  //  }

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    //    String newValue = "Time since boot: " + String(millis() / 1000);
    //    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    //    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  } else if (doScan) {
    lookForDevice();
  }

} // End of loop
