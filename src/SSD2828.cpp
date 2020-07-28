#include "SSD2828.h"

//-----------------------------------------------------------------------------
SSD2828::SSD2828(const uint8_t CSp, const uint8_t RSTp, const uint8_t SDIp, const uint8_t SCKp) {
  CS_2828 = CSp;
  RST_2828 = RSTp;
  SDI_2828 = SDIp;
  SCLK_2828 = SCKp;
}

//-----------------------------------------------------------------------------
void SSD2828::reset(void)
{

  // GPIO init

  pinMode(SDI_2828,OUTPUT);
  digitalWrite(SDI_2828,1);

  pinMode(SCLK_2828,OUTPUT);
  digitalWrite(SCLK_2828,1);

  pinMode(CS_2828,OUTPUT);
  digitalWrite(CS_2828,1);

  pinMode(RST_2828,OUTPUT);
  digitalWrite(RST_2828,1);

  // chip reset

  digitalWrite(RST_2828,0);
  delay(30);

  digitalWrite(RST_2828,1);
  delay(50);

}
//-----------------------------------------------------------------------------
static inline void spi_delay(void)
{
  // 128 NOPS should bring the SPI speed well below 2.3 MHz on
  // a Teensy 4.0. On AVR devices, even slower.

  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");

  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
  asm("nop;nop;nop;nop;nop;nop;nop;nop;");
}
//-----------------------------------------------------------------------------
void SSD2828::SPI_3W_SET_Cmd(uint16_t Sdata)
{
  uint8_t i;
  digitalWrite(CS_2828,0);
  spi_delay();
  digitalWrite(SDI_2828,0);
  spi_delay();
  digitalWrite(SCLK_2828 ,0);
  spi_delay();
  digitalWrite(SCLK_2828 ,1);
  spi_delay();
  for(i=8; i>0; i--)
  {
    if(Sdata&0x80)
      digitalWrite(SDI_2828,1);
    else
      digitalWrite(SDI_2828,0);
    spi_delay();
    digitalWrite(SCLK_2828 ,0);
    spi_delay();
    digitalWrite(SCLK_2828 ,1);
    spi_delay();
    Sdata <<= 1;
  }
  digitalWrite(SCLK_2828 ,0);
  spi_delay();
  digitalWrite(CS_2828,1);
  spi_delay();
}
//-----------------------------------------------------------------------------
void SSD2828::SPI_3W_SET_PAs(uint16_t Sdata)
{
  uint8_t i;
  digitalWrite(CS_2828,0);
  spi_delay();
  digitalWrite(SDI_2828,1);
  spi_delay();
  digitalWrite(SCLK_2828 ,0);
  spi_delay();
  digitalWrite(SCLK_2828 ,1);
  spi_delay();
  for(i=8; i>0; i--)
  {
    if(Sdata&0x80)
      digitalWrite(SDI_2828,1);
    else
      digitalWrite(SDI_2828,0);
    spi_delay();
    digitalWrite(SCLK_2828 ,0);
    spi_delay();
    digitalWrite(SCLK_2828 ,1);
    spi_delay();
    Sdata <<= 1;
  }
  digitalWrite(SCLK_2828 ,0);
  spi_delay();
  digitalWrite(CS_2828,1);
  spi_delay();
}
//-----------------------------------------------------------------------------
void SSD2828::SPI_WriteData(uint8_t value)
{
  SPI_3W_SET_PAs(value);
}
//-----------------------------------------------------------------------------
void SSD2828::SPI_WriteCmd(uint8_t value)
{
  SPI_3W_SET_Cmd(value);
}
//-----------------------------------------------------------------------------
void SSD2828::GP_COMMAD_PA(uint16_t num)
{
  SPI_WriteCmd(0xbc);
  SPI_WriteData(num&0xff);
  SPI_WriteData((num>>8)&0xff);
  SPI_WriteCmd(0xbf);
}
//-----------------------------------------------------------------------------
void SSD2828::initialize(void)
{
  SPI_WriteCmd(0xb7);
  SPI_WriteData(0x50);  // 50=TX_CLK 70=PCLK
  SPI_WriteData(0x00);  // Configuration Register

  SPI_WriteCmd(0xb8);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);  // VC(Virtual ChannelID) Control Register

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x00);  // 1=PLL disable
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xBA);   // PLL=(TX_CLK/MS)*NS
  SPI_WriteData(0x20);  // 14,D7-0=NS(0x01 : NS=1)
  SPI_WriteData(0x82);  // 42,D15-14=PLL00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)

  SPI_WriteCmd(0xBB);   // LP Clock Divider LP clock = 400MHz / LPD / 8 = 480 / 8/ 8 = 7MHz
  SPI_WriteData(0x07);  // D5-0=LPD=0x1 ¨C Divide by 2
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x01);  // 1=PLL disable
  SPI_WriteData(0x00);

  //MIPI lane configuration

  SPI_WriteCmd(0xDE);
  SPI_WriteData(0x00);  // 11=4LANE 10=3LANE 01=2LANE 00=1LANE
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xc9);
  SPI_WriteData(0x02);
  SPI_WriteData(0x23);  // p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue

  delay(100);

  SPI_WriteCmd(0xCA);
  SPI_WriteData(0x01);  // CLK Prepare
  SPI_WriteData(0x23);  // Clk Zero

  SPI_WriteCmd(0xCB);   // local_write_reg(addr=0xCB,data=0x0510)
  SPI_WriteData(0x10);  // Clk Post
  SPI_WriteData(0x05);  // Clk Per

  SPI_WriteCmd(0xCC);   // local_write_reg(addr=0xCC,data=0x100A)
  SPI_WriteData(0x05);  // HS Trail
  SPI_WriteData(0x10);  // Clk Trail

  //LCD driver initialization

  SPI_WriteCmd(0xB7);
  SPI_WriteData(0x50);  // 10=TX_CLK 30=PCLK
  SPI_WriteData(0x02);

  SPI_WriteCmd(0xBD);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(1);
  SPI_WriteData(0x10);


  GP_COMMAD_PA(2);
  SPI_WriteData(0xCD);
  SPI_WriteData(0xAA);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x41);
  SPI_WriteData(0x34);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x30);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x39);
  SPI_WriteData(0x11);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x32);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x33);
  SPI_WriteData(0x38);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x35);
  SPI_WriteData(0x24);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x4F);
  SPI_WriteData(0x35);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x4E);
  SPI_WriteData(0x27);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x41);
  SPI_WriteData(0x56);

  GP_COMMAD_PA(9);
  SPI_WriteData(0x55);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);

  GP_COMMAD_PA(17);
  SPI_WriteData(0x56);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);
  SPI_WriteData(0x00);
  SPI_WriteData(0x0F);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x65);
  SPI_WriteData(0x08);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x3A);
  SPI_WriteData(0x08);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x36);
  SPI_WriteData(0x49);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x67);
  SPI_WriteData(0x82);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x69);
  SPI_WriteData(0x20);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x6C);
  SPI_WriteData(0x80);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x6D);
  SPI_WriteData(0x01);

  GP_COMMAD_PA(20);
  SPI_WriteData(0x53);
  SPI_WriteData(0x1F);
  SPI_WriteData(0x19);
  SPI_WriteData(0x15);
  SPI_WriteData(0x11);
  SPI_WriteData(0x11);
  SPI_WriteData(0x11);
  SPI_WriteData(0x12);
  SPI_WriteData(0x14);
  SPI_WriteData(0x15);
  SPI_WriteData(0x11);
  SPI_WriteData(0x0D);
  SPI_WriteData(0x0B);
  SPI_WriteData(0x0B);
  SPI_WriteData(0x0D);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x08);
  SPI_WriteData(0x04);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(20);
  SPI_WriteData(0x54);
  SPI_WriteData(0x1F);
  SPI_WriteData(0x19);
  SPI_WriteData(0x15);
  SPI_WriteData(0x11);
  SPI_WriteData(0x11);
  SPI_WriteData(0x11);
  SPI_WriteData(0x13);
  SPI_WriteData(0x15);
  SPI_WriteData(0x16);
  SPI_WriteData(0x11);
  SPI_WriteData(0x0D);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x0E);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x0C);
  SPI_WriteData(0x08);
  SPI_WriteData(0x04);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x6B);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x58);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x73);
  SPI_WriteData(0xF0);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x76);
  SPI_WriteData(0x40);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x77);
  SPI_WriteData(0x04);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x74);
  SPI_WriteData(0x17);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x5E);
  SPI_WriteData(0x03);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x68);
  SPI_WriteData(0x10);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x6A);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x28);
  SPI_WriteData(0x31);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x29);
  SPI_WriteData(0x21);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x63);
  SPI_WriteData(0x04);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x27);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x7C);
  SPI_WriteData(0x80);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x2E);
  SPI_WriteData(0x05);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x4C);
  SPI_WriteData(0x80);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x50);
  SPI_WriteData(0xC0);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x78);
  SPI_WriteData(0x6E);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x2D);
  SPI_WriteData(0x31);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x49);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(2);
  SPI_WriteData(0x4D);
  SPI_WriteData(0x00);

  GP_COMMAD_PA(1);
  SPI_WriteData(0x11);
  delay(120);
  GP_COMMAD_PA(1);
  SPI_WriteData(0x29);
  delay(120);

  SPI_WriteCmd(0xb7);
  SPI_WriteData(0x50);
  SPI_WriteData(0x00);  // Configuration Register

  SPI_WriteCmd(0xb8);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);  // VC(Virtual ChannelID) Control Register

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x00);  // 1=PLL disable
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xBA);
  SPI_WriteData(0x20);  // 14,D7-0=NS(0x01 : NS=1)
  SPI_WriteData(0x82);  // 42,D15-14=PLL00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)

  SPI_WriteCmd(0xBB);   // LP Clock Divider LP clock = 400MHz / LPD / 8 = 480 / 8/ 8 = 7MHz
  SPI_WriteData(0x07);  // D5-0=LPD=0x1 ¨C Divide by 2
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x01);  // 1=PLL disable
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xc9);
  SPI_WriteData(0x02);
  SPI_WriteData(0x23);  // p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
  delay(100);

  SPI_WriteCmd(0xCA);
  SPI_WriteData(0x01);  // CLK Prepare
  SPI_WriteData(0x23);  // Clk Zero

  SPI_WriteCmd(0xCB);   // local_write_reg(addr=0xCB,data=0x0510)
  SPI_WriteData(0x10);  // Clk Post
  SPI_WriteData(0x05);  // Clk Per

  SPI_WriteCmd(0xCC);   // local_write_reg(addr=0xCC,data=0x100A)
  SPI_WriteData(0x05);  // HS Trail
  SPI_WriteData(0x10);  // Clk Trail

  SPI_WriteCmd(0xD0);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);

  //RGB interface configuration

  SPI_WriteCmd(0xB1);
  SPI_WriteData(0x18);  // HSPW 07
  SPI_WriteData(0x02);  // VSPW 05

  SPI_WriteCmd(0xB2);
  SPI_WriteData(0xa0);  // HBPD 0x64=100
  SPI_WriteData(0x0a);  // VBPD 8 ¼õÐ¡ÏÂÒÆ

  SPI_WriteCmd(0xB3);
  SPI_WriteData(0xa0);  // HFPD 8
  SPI_WriteData(0x0c);  // VFPD 10

  SPI_WriteCmd(0xB4);   // Horizontal active period 720=02D0
  SPI_WriteData(0x90);  // 013F=319 02D0=720
  SPI_WriteData(0x01);

  SPI_WriteCmd(0xB5);   // Vertical active period 1280=0500
  SPI_WriteData(0x00);  // 01DF=479 0500=1280
  SPI_WriteData(0x05);


  SPI_WriteCmd(0xB6);   // RGB CLK  16BPP=00 18BPP=01
  SPI_WriteData(0x0b);  // D7=0 D6=0 D5=0  D1-0=11 ¨C 24bpp   //07
  SPI_WriteData(0x00);  // D15=VS D14=HS D13=CLK D12-9=NC D8=0=Video with blanking packet. 00-F0

  //MIPI lane configuration

  SPI_WriteCmd(0xDE);
  SPI_WriteData(0x03);  // 11=4LANE 10=3LANE 01=2LANE 00=1LANE
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xD6);   // 05=BGR  04=RGB
  SPI_WriteData(0x05);  // D0=0=RGB 1:BGR D1=1=Most significant byte sent first
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xDB);
  SPI_WriteData(0x58);
  SPI_WriteData(0x00);


  SPI_WriteCmd(0xB7);
  SPI_WriteData(0x6B);
  SPI_WriteData(0x02);

}


