#include "ER-TFT0784_t3.h"
#include "SSD2828.h"
boolean ER_TFT0784_t3::begin(uint32_t spi_clock) {

  // SSD2828 setup.
  // host GPIO pin numbers for the SSD2828 pin: CS, RST, SDI and SCK
  SSD2828 ssd2828 = SSD2828(_cs2828p, _rst2828p, _sdi2828p, _sck2828p);
  ssd2828.reset();
  ssd2828.initialize();

  return RA8876_t3::begin(spi_clock);
}
