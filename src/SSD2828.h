#ifndef __SSD2828_H__
#define __SSD2828_H__

///////////////////////////////////////////////////////////////////////////////
//
// SSD2828 MIPI Bridge - Initialization
//
// 2020-07-26 : Adapted for use with RA8876_t3 library by Dean Blackketter
// 2020-04-14 : Ported to Arduino by Craig Iannello
//              Original by Javen Liu
//
// Hard-coded to work with this particular LCD panel:
//
// Brand: EASTRISING
// Model: ER-TFTM0784-1
//        ( 400 x 1280 dots 7.84" color IPS tft lcd )
//
// This code was only slightly modified from the 8051 example code
// written by JAVEN LIU. In his example, he bit-bangs the SPI to the
// SSD2828, and since this only needs to be done on powerup and doesnt
// need to be performant,  I decided to keep it that way to save resources.
//
// I did change the GPIO stuff to be Arduino-oriented rather than Keil 8051.
//
///////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <stdint.h>

class SSD2828 {
public:
	SSD2828(const uint8_t CSp = 24, const uint8_t RSTp = 25, const uint8_t SDIp = 26, const uint8_t SCKp = 27);

  void reset();
  void initialize();
  void release();

private:
  uint8_t CS_2828;
  uint8_t RST_2828;
  uint8_t SDI_2828;
  uint8_t SCLK_2828;

  void SPI_3W_SET_Cmd(uint16_t Sdata);
  void SPI_3W_SET_PAs(uint16_t Sdata);
  uint8_t SPI_ReadData(void);
  void SPI_WriteData(uint8_t value);
  void SPI_WriteCmd(uint8_t value);
  void GP_COMMAD_PA(uint16_t num);
};

#endif
