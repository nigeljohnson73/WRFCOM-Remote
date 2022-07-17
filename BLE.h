#ifndef _BLE_h
#define _BLE_h

#include "App.h"

class TrBLE {
  public:
    TrBLE();

    void begin();
    void loop();

    bool isEnabled() {
      return _enabled;
    };

    bool isConnected();

    // Core - WRFCOM_CORE_SERVICE_UUID
    String getName() {
      return _DEVICE_NAME_;
    }
    double getBattery(); // Percentage // BATTERY_CHARACTERISTIC_UUID
    String getWifiMode();              // WIFI_CHARACTERISTIC_UUID
    String getIpAddress();             // IPADDR_CHARACTERISTIC_UUID
    bool isArmed();                    // ARMED_CHARACTERISTIC_UUID
    void setArmed(bool tf);
    bool isLogging();                  // LOGGING_CHARACTERISTIC_UUID
    void setLogging(bool tf);


    // Capability - WRFCOM_CAPABILITY_SERVICE_UUID
    bool hasBms();      // BMSENABLED_CHARACTERISTIC_UUID
    bool hasGps();      // BMSENABLED_CHARACTERISTIC_UUID
    bool hasEmu();      // EMUENABLED_CHARACTERISTIC_UUID
    bool hasImu();      // IMUENABLED_CHARACTERISTIC_UUID
    bool hasSrv();      // SRVENABLED_CHARACTERISTIC_UUID
    bool hasRtc();      // RTCENABLED_CHARACTERISTIC_UUID

    bool isGpsLocked(); // hasGps() && GPSLOCKED_CHARACTERISTIC_UUID
    bool hasTemp();     // hasEMU() && CTEMPERATURE_CHARACTERISTIC_UUID
    bool hasPressure(); // hasEMU() && CPRESSURE_CHARACTERISTIC_UUID
    bool hasAcc();      // hasIMU() && CACC_CHARACTERISTIC_UUID
    bool hasGyro();     // hasIMU() && CGYRO_CHARACTERISTIC_UUID
    bool hasMag();      // hasIMU() && CMAG_CHARACTERISTIC_UUID

    // GPS - WRFCOM_GPS_SERVICE_UUID
    int getSiv(); //                     // SIV_CHARACTERISTIC_UUID
    double getLatitude(); // degrees N   // LATITUDE_CHARACTERISTIC_UUID
    double getLongitude(); // degrees E  // LONGITUDE_CHARACTERISTIC_UUID
    double getAltitude(); // m           // ALTITUDE_CHARACTERISTIC_UUID

    // EMU - WRFCOM_EMU_SERVICE_UUID
    double getTemp(); // Celcius         // TEMPERATURE_CHARACTERISTIC_UUID
    double getPressure(); // hPa         // PRESSURE_CHARACTERISTIC_UUID

    // IMU - WRFCOM_IMU_SERVICE_UUID
    // IMU - Acc
    double getAccX(); // m/s2            // ACCX_CHARACTERISTIC_UUID
    double getAccY();                    // ACCY_CHARACTERISTIC_UUID
    double getAccZ();                    // ACCZ_CHARACTERISTIC_UUID

    // IMU - Gyro
    double getGyroX(); // degrees/s      // GYROX_CHARACTERISTIC_UUID
    double getGyroY();                   // GYROY_CHARACTERISTIC_UUID
    double getGyroZ();                   // GYROZ_CHARACTERISTIC_UUID

    // IMU - Mag
    double getMagX(); // uTelsa          // MAGX_CHARACTERISTIC_UUID
    double getMagY();                    // MAGY_CHARACTERISTIC_UUID
    double getMagZ();                    // MAGY_CHARACTERISTIC_UUID

  private:
    bool _enabled = false;

};

extern TrBLE BLE;

#endif
