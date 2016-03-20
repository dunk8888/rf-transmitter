#ifndef SPI_WRAPPER_H
#define SPI_WRAPPER_H

extern "C" {
  void sw_spi_init();
  void sw_spi_begin();
  unsigned char sw_spi_transfer(unsigned char data);
  void sw_spi_end();
}

#endif
