#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "t400.h"


namespace ChargeStatus {
  
  void setup();
  
  enum State {
     DISCHARGING = 0,    // VBUS=0
     CHARGING = 1,       // VBUS=1, BATT_STAT=0
     CHARGED = 2,        // VBUS=1, BATT_STAT=1, VBATT_SENSE=?
     NO_BATTERY = 3,     // VBUS=1, BATT_STAT=1, VBATT_SENSE=?
  };
  
  // Get the battery status
  // @return 
  State get();
  
  // Get a measurement of the battery leve
  // @return 0 = empty, 4= full
  uint8_t getBatteryLevel();
}


namespace Backlight {
  
  inline void setup() {
    pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  } 
    
  inline void set(uint8_t level) {  
    if(level > 0) {
      digitalWrite(LCD_BACKLIGHT_PIN, LOW);
    }
    else {
      digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
    }
  }

}

namespace Display {
  
  void setup();
  
  void draw(uint8_t mode);
  
  void clear();
}


#endif
