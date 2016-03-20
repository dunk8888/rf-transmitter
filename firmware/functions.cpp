#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <avr/io.h>

#include <Arduino.h>
#include <PaxInstruments-U8glib.h>
#include "t400.h"
#include "functions.h"

namespace Display {
  
// Graphical LCD
U8GLIB_PI13264  u8g(LCD_CS, LCD_A0, LCD_RST); // Use HW-SPI
  

void setup() {
  u8g.setContrast(LCD_CONTRAST); // Set contrast level
  
  u8g.setRot180(); // Rotate screen
  u8g.setColorIndex(1); // Set color mode to binary
  u8g.setFont(u8g_font_5x8r); // Select font. See https://code.google.com/p/u8glib/wiki/fontsize
}

void draw(uint8_t mode) {

  // Graphic commands to redraw the complete screen should be placed here
  static char buf[8];

  uint8_t page = 0;
  u8g.firstPage();  // Update the screen
  do {

    // Top line
    if (page == 7) {
      u8g.drawStr(40,   6,  "Fly By Night");
      u8g.drawLine(0, 7, 132, 7);
    }
    
    else if(page == 5) {  
      if(mode == COMMAND_HELLO) {
        u8g.drawStr(45,   24,   "STANDBY");
      }
      else if(mode == COMMAND_SQUACK) {
        u8g.drawStr(43,   24,  "BEGIN SHOW");
      }
    }
    
    else {

    }    

    
    page++;
  } 
  
  while( u8g.nextPage() );
}

void clear() {
  // Clear the screen
  u8g.firstPage();  
  do {
  } while( u8g.nextPage() );
}

}


namespace ChargeStatus {
  
void setup() {
  // Set VBAT_EN high to enable VBAT_SENSE readings
  pinMode(VBAT_EN, OUTPUT);
  digitalWrite(VBAT_EN, HIGH);

  // Configure the batt stat pin as an input
//  analogReference(DEFAULT);
  pinMode(BATT_STAT, INPUT);
  pinMode(VBAT_SENSE, INPUT);  

  // enable the VBUS pad
  USBCON |= (1<<OTGPADE);
}

State get() {
  // We want to output one of these states:
//   DISCHARGING = 0,    // VBUS=0
//   CHARGING = 1,       // VBUS=1, BATT_STAT<.7V
//   CHARGED = 2,        // VBUS=1, .8V<BATT_STAT<1.2V
//   NO_BATTERY = 3,     // VBUS=1, 3V<BATT_STAT

  bool usbConnected = USBSTA & _BV(VBUS);

  uint16_t battStatCounts = analogRead(BATT_STAT);
  
  #define BATT_CHARGING_COUNTS_MAX  217 // 1024/3.3*.7
  #define BATT_DISCONNECTED_COUNTS_MIN 248 // 1024/3.3*.8
  #define BATT_DISCONNECTED_COUNTS_MAX 372 // 1024/3.3*.1.2

  if(!usbConnected) {
    return DISCHARGING;
  }
  else if(battStatCounts < BATT_CHARGING_COUNTS_MAX) {
    return CHARGING;
  }
  else if(BATT_DISCONNECTED_COUNTS_MIN < battStatCounts
        && battStatCounts < BATT_DISCONNECTED_COUNTS_MAX) {
    return NO_BATTERY;
  }
  else {
    // default to this
    return CHARGED;
  }
}

uint8_t getBatteryLevel() {
  // VBAT_SENSE_V= 34 × VBAT/(34 + 18.7)
  // VBAT_SENSE_COUNTS = VBAT_SENSE_V / 3.3 * 1024
  
  #define VBAT_SENSE_FULL 820   // 4.1V
  #define VBAT_SENSE_EMPTY 720  // 3.6V
  
  // Note: We'll divide this into 5 sections so that the user gets a full battery for a little bit.
  uint16_t vbatSenseCounts = analogRead(VBAT_SENSE);
  uint8_t batteryLevel = ((vbatSenseCounts - VBAT_SENSE_EMPTY)*5)/(VBAT_SENSE_FULL - VBAT_SENSE_EMPTY);
  
  return batteryLevel<5?batteryLevel:4;
}

}
