#ifndef _LCD_h
#define _LCD_h

#include "App.h"

class TrLCD {
  public:
    TrLCD();

    void begin();
    void loop();

    bool isEnabled() {
      return _enabled;
    };

  private:
    bool _enabled = false;

};

extern TrLCD LCD;

#endif
