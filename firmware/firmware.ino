#include <bk2425.h>
#include <bk2425_util.h>
#include <bk2425_registers.h>
#include <crc.h>
#include "spi_wrapper.h"


// Import libraries
#include <Wire.h>       // i2c
#include <SPI.h>

#include <PaxInstruments-U8glib.h>

#include "power.h"            // Manage board power
#include "buttons.h"          // User buttons
#include "functions.h"        // Misc. functions
#include "t400.h"             // Board definitions

#define BUFF_MAX         80   // Size of the character buffer


boolean backlightEnabled = true;

unsigned char transmitterAddress[5] = {0x02,0x03,0x04,0x05,0x06};
unsigned char receiverAddress[5] = {0x01,0x02,0x03,0x04,0x05};

uint8_t mode;

unsigned char commandData[COMMAND_LENGTH];


//--------------------------------------------------------------
// User interface
//--------------------------------------------------------------

bool bk2425_init_nrf(const uint8_t* address) {
    _delay_ms(100);
    
    //--------------------------------------------------------------
    // Configure Bank 0 registers
    //--------------------------------------------------------------
//    bk2425_select_register_bank(0);

//    bk2425_cmd_write_register(BK2425_RX_PW_P0,        BK2425_RX_PW_PN_LENGTH(BK2425_MAX_PACKET_LEN));  // Max packet length 32
    bk2425_cmd_write_register(BK2425_RX_PW_P0,        BK2425_RX_PW_PN_LENGTH(BK2425_FIXED_PACKET_LEN));  // Fixed packet length 7
    
    bk2425_cmd_write_register(BK2425_STATUS,        BK2425_STATUS_RX_DR | BK2425_STATUS_TX_DS | BK2425_STATUS_MAX_RT);
    bk2425_cmd_write_register(BK2425_RF_SETUP,      BK2425_RF_SETUP_RF_DR_LOW | BK2425_RF_SETUP_RF_RF_PWR(0x3)); // 250Kbps
//    bk2425_cmd_write_register(BK2425_RF_SETUP,      BK2425_RF_SETUP_RF_RF_PWR(0x3) | BK2425_RF_SETUP_RF_LNA_HCURR); // 1Mbps
//    bk2425_cmd_write_register(BK2425_RF_SETUP,      BK2425_RF_SETUP_RF_DR_HIGH | BK2425_RF_SETUP_RF_RF_PWR(0x3) | BK2425_RF_SETUP_RF_LNA_HCURR); // 2Mbps
    bk2425_cmd_write_register(BK2425_RF_CH,         BK2425_RF_CH_RF_CH(0x17));
    bk2425_cmd_write_register(BK2425_SETUP_RETR,    BK2425_SETUP_RETR_ARD(0xF) | BK2425_SETUP_RETR_ARC(0xF)); // TODO: don't bother writing, feature disabled
    bk2425_cmd_write_register(BK2425_SETUP_AW,      BK2425_SETUP_AW_AW(0x3)); // 5-byte address
    bk2425_cmd_write_register(BK2425_EN_RXADDR,     BK2425_EN_RXADDR_ERX_P0); // Only enable pipe 0 (address 0)
    bk2425_cmd_write_register(BK2425_EN_AA,         0x00); // 0x00 to disable AA (was 0x3F)
    bk2425_cmd_write_register(BK2425_CONFIG,        BK2425_CONFIG_MASK_RX_DR | BK2425_CONFIG_MASK_TX_DS | BK2425_CONFIG_MASK_MAT_RT | BK2425_CONFIG_EN_CRC | BK2425_CONFIG_CRCO | BK2425_CONFIG_PWR_UP);

    bk2425_cmd_write_register_bytes(BK2425_RX_ADDR_P0,    address, 5);

    
    // And we're done!
    return true;
}


void sendMessage(uint8_t message) {
  commandData[0] = message;
  commandData[1] = 1;    // 60 minute on time
  commandData[2] = 4;     // 4 second sleep time
  commandData[3] = 255;   // full brightness
  commandData[4] = 0;     // reserved
  commandData[5] = 0;     // reserved

  resetCRC();
  for(uint8_t i = 0; i < COMMAND_LENGTH-1; i++) {
    updateCRC(commandData[i]);
  }
  commandData[COMMAND_LENGTH-1] = getCRC();
  
  bk2425_send_packet(receiverAddress, commandData, COMMAND_LENGTH);

  digitalWrite(RADIO_CE, HIGH);
  delay(1);
  digitalWrite(RADIO_CE, LOW);
}



// This function runs once. Use it for setting up the program state.
void setup(void) {
  Power::setup();
  ChargeStatus::setup();

  Serial.begin(9600);
  
  Backlight::setup();
  Backlight::set(backlightEnabled);

  mode = COMMAND_HELLO;
  
  Display::setup();
  Display::draw(mode);

  Buttons::setup();

  

  sw_spi_init();
  pinMode(RADIO_CE, OUTPUT);
  digitalWrite(RADIO_CE, LOW);
  
  if(!bk2425_init_nrf(transmitterAddress)) {
    Serial.println("Error starting radio");
  }
  
  bk2425_begin_tx();


}


// This function is called periodically, and performs slow tasks:
// Taking measurements
// Updating the screen
void loop() {
  bool needsRedraw = false;
  
  sendMessage(mode);


  if(Buttons::pending()) {
    uint8_t button = Buttons::getPending();
    
    if(button == Buttons::BUTTON_POWER) { // Disable power
//      Display::clear();
//      Backlight::set(0);
//      Power::shutdown();
    }
    else if(button == Buttons::BUTTON_A) {  // Toggle squack/hello mode
      if(mode == COMMAND_HELLO) {
        mode = COMMAND_SQUACK;
      }
      else {
        mode = COMMAND_HELLO;
      }
    }
    else if(button == Buttons::BUTTON_B) { // 
    }
    else if(button == Buttons::BUTTON_C) { // 
    }
    else if(button == Buttons::BUTTON_D) { // 
    }
    else if(button == Buttons::BUTTON_E) { // 
      backlightEnabled = !backlightEnabled;
      Backlight::set(backlightEnabled);
    }

    needsRedraw = true;
  }

  if(needsRedraw) {
    Display::draw(mode);
  }
}
