#ifndef _IND_h
#define _IND_h

#include "App.h"

class TrIND {
  public:
    TrIND();

    void begin();
    void loop();

    void setScanning();

    bool isEnabled() {
      return _enabled;
    };

    void blink(unsigned n) {
      _n_blips = n + 1;
    }
    
  private:
    bool _enabled = false;
    bool _has_stick = false;

    int _stick_brightness = 1; // 0-31 apparently
    
    unsigned long _sequence_started;
    unsigned _n_blips;
    bool _blip;
    unsigned long _blip_started;
    unsigned long _sequence_duration = 1000;

    unsigned _blip_on_duration = 150;
    unsigned _blip_off_duration = 50;
    unsigned long _blip_duration;
};

extern TrIND IND;

#endif
