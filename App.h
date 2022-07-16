#ifndef _APP_h
#define _APP_h

#define VERSION "v0.1a"

// Preload our config to override any of the parameters
#include "myConfig.h"

#ifndef _DEVICE_NAME_
#error "You must define _DEVICE_NAME_ in myConfig.h"
#endif

#ifndef _USE_BMS_
#define _USE_BMS_ true
#endif

#ifndef LIPO_SIZE
#define LIPO_SIZE 0x10
#endif
// Options:
//  LC709203F_APA_100MAH = 0x08,
//  LC709203F_APA_200MAH = 0x0B,
//  LC709203F_APA_500MAH = 0x10,
//  LC709203F_APA_1000MAH = 0x19,
//  LC709203F_APA_2000MAH = 0x2D,
// LC709203F_APA_3000MAH = 0x36,

#include <Arduino.h>
#include "BLE.h"
#include "BMS.h"
#include "IND.h"
#include "LCD.h"

#define _DEBUG_ true
#define _XDEBUG_ false

#endif
