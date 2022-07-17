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

	void setScanning();

protected:
	void nextPage();
	void toggleArm();
	void toggleLog();
	
  private:
    bool _enabled = false;
    bool _buttons_enabled = false;
    int _current_page = 0;

};

extern TrLCD LCD;

#endif
