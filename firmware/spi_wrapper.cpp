#include "arduino.h"
#include "t400.h"
#include "spi_wrapper.h"
#include <SPI.h>

void sw_spi_begin() {
  digitalWrite(RADIO_CSN, LOW);
}

void sw_spi_end() {
  digitalWrite(RADIO_CSN, HIGH);
}

void sw_spi_init() {
  SPI.begin();
  
  pinMode(RADIO_CSN, OUTPUT);
  digitalWrite(RADIO_CSN, HIGH);
}

unsigned char sw_spi_transfer(unsigned char data) {
  return SPI.transfer(data);
}
