#ifndef ER_TFT0784
#define ER_TFT0784
#include "RA8876_t3.h"

class ER_TFT0784_t3 : public RA8876_t3 {
	public:
		ER_TFT0784_t3(const uint8_t CSp = 10,
								  const uint8_t RSTp = 8,
								  const uint8_t mosi_pin = 11,
								  const uint8_t sclk_pin = 13,
								  const uint8_t miso_pin = 12,
								  const uint8_t cs_2828_pin = 24,
								  const uint8_t rst_2828_pin = 25,
								  const uint8_t sdi_2828_pin = 26,
								  const uint8_t sck_2828_pin = 27) :
				RA8876_t3(CSp, RSTp, mosi_pin, sclk_pin, miso_pin)
		{
			displayDimensions(400,1280);
			_cs2828p  = cs_2828_pin;
			_rst2828p = rst_2828_pin;
			_sdi2828p = sdi_2828_pin;
			_sck2828p = sck_2828_pin;
		}

		boolean begin(uint32_t spi_clock=SPIspeed) override;

	private:
		uint8_t _cs2828p;
		uint8_t _rst2828p;
		uint8_t _sdi2828p;
		uint8_t _sck2828p;
};
#endif

