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
	void buttonPressA();
	void buttonPressB();
	void buttonPressC();
	void buttonHoldA();
	void buttonHoldB();
	void buttonHoldC();
	
  private:
    bool _enabled = false;
    bool _buttons_enabled = false;
    int _current_page = 0;
    unsigned long _button_hold_millis = 1000;

};

extern TrLCD LCD;

#endif
