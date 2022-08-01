#ifndef _NOW_h
#define _NOW_h

#include "App.h"

class TrNOW {
  public:
    TrNOW();

    void begin();
    void loop();

    bool isConnected();
    bool isEnabled() {
      return _enabled;
    };

    String getName() {
      return _DEVICE_NAME_;
    }
    double getBattery();

    bool isArmed();
    void setArmed(bool tf);
    bool isLogging();
    void setLogging(bool tf);


    // Capability - WRFCOM_CAPABILITY_SERVICE_UUID
    bool hasBms();
    bool hasGps();
    bool hasEmu();
    bool hasImu();
    bool hasSrv();
    bool hasRtc();

    bool isGpsLocked();
    bool hasTemp();
    bool hasPressure();
    bool hasAcc();
    bool hasGyro();
    bool hasMag();

    // GPS
    int getSiv();
    double getLatitude(); // degrees N
    double getLongitude(); // degrees E
    double getAltitude(); // m above MSL

    // EMU
    double getTemp(); // Celcius
    double getPressure(); // hPa

    // IMU - Acc
    double getAccX(); // m/s2
    double getAccY();
    double getAccZ();

    // IMU - Gyro
    double getGyroX(); // degrees/s
    double getGyroY();
    double getGyroZ();

    // IMU - Mag
    double getMagX(); // uTelsa
    double getMagY();
    double getMagZ();

  private:
    bool _enabled = false;

};

extern TrNOW NOW;

#endif
