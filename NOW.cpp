#include "NOW.h"
TrNOW NOW;
TrNOW::TrNOW() {};

#if !_USE_NOW_

void TrNOW::begin() {}
void TrNOW::loop() {}
bool TrNOW::isConnected() {
  return false;
};



#else //_USE_NOW_
#include "NOW_SVC.h"

now_status_packet status_packet;
now_command_packet command_packet;
bool now_connected = false;




bool TrNOW::isConnected() {
  return now_connected;
};

// Core
double TrNOW::getBattery() {
  return status_packet.battery;
}

bool TrNOW::isArmed() {
  return status_packet.is_arm;
}
bool TrNOW::isLogging() {
  return status_packet.is_log;
}
void TrNOW::setArmed(bool tf) {
  Serial.print("TrNOW::setArmed(");
  Serial.print(tf ? "TRUE" : "FALSE");
  Serial.print(")");
  Serial.println();

  command_packet.cmd = 0x01;
  command_packet.val = tf;
  // TBD
  // Send it
}
void TrNOW::setLogging(bool tf) {
  Serial.print("TrNOW::setLogging(");
  Serial.print(tf ? "TRUE" : "FALSE");
  Serial.print(")");
  Serial.println();

  command_packet.cmd = 0x02;
  command_packet.val = tf;
  // TBD
  // Send it
}





// Capability
bool TrNOW::hasBms() {
  return status_packet.has_bms;
}
bool TrNOW::hasGps() {
  return status_packet.has_gps;
}
bool TrNOW::isGpsLocked() {
  return hasGps() && status_packet.is_gps_locked;
}
bool TrNOW::hasEmu() {
  return status_packet.has_emu;
}
bool TrNOW::hasImu() {
  return status_packet.has_imu;
}
bool TrNOW::hasRtc() {
  return status_packet.has_rtc;
}
bool TrNOW::hasSrv() {
  return status_packet.has_srv;
}
bool TrNOW::hasTemp() {
  return hasEmu() && status_packet.has_temp;
}
bool TrNOW::hasPressure() {
  return hasEmu() && status_packet.has_pressure;
}
bool TrNOW::hasAcc() {
  return hasImu() && status_packet.has_acc;
}
bool TrNOW::hasGyro() {
  return hasImu() && status_packet.has_gyro;
}
bool TrNOW::hasMag() {
  return hasImu() && status_packet.has_mag;
}




// GPS
int TrNOW::getSiv() {
  return status_packet.siv;
}
double TrNOW::getLatitude() {
  return status_packet.latitude;
}
double TrNOW::getLongitude() {
  return status_packet.longitude;
}
double TrNOW::getAltitude() {
  return status_packet.altitude;
}





// EMU
double TrNOW::getTemp() {
  return status_packet.temp;
}
double TrNOW::getPressure() {
  return status_packet.pressure;
}

// IMU - ACC
double TrNOW::getAccX() {
  return status_packet.acc_x;
}
double TrNOW::getAccY() {
  return status_packet.acc_y;
}
double TrNOW::getAccZ() {
  return status_packet.acc_z;
}





// IMU - GYRO
double TrNOW::getGyroX() {
  return status_packet.gyro_x;
}
double TrNOW::getGyroY() {
  return status_packet.gyro_y;
}
double TrNOW::getGyroZ() {
  return status_packet.gyro_z;
}





// IMU - MAG
double TrNOW::getMagX() {
  return status_packet.mag_x;
}
double TrNOW::getMagY() {
  return status_packet.mag_y;
}
double TrNOW::getMagZ() {
  return status_packet.mag_z;
}






void TrNOW::begin() {
  Serial.print("NOW initialised: ");
  Serial.print(sizeof(status_packet));
  Serial.print(" byte status packets, ");
  Serial.print(sizeof(command_packet));
  Serial.print(" byte command packets ");
  Serial.println();

  _enabled = true;
}

void TrNOW::loop() {
}


#endif // !_USE_NOW_
