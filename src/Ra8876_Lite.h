//**************************************************************//
/*
File Name : Ra8876_Lite.h                                   
Author    : RAiO Application Team                             
Edit Date : 12/29/2015
Version   : v1.0
*
* Modified Version of: File Name : Ra8876_Lite.h                                   
 *			Author    : RAiO Application Team                             
 *			Edit Date : 09/13/2017
 * 	  	     : For Teensy 3.x and T4
 *                   : By Warren Watson
 *                   : 06/07/2018 - 11/31/2019
 *                   : Copyright (c) 2017-2019 Warren Watson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
//**************************************************************//
#include "Arduino.h"
#include "SPI.h"

#ifndef _RA8876_LITE
#define _RA8876_LITE

typedef int8_t		rs8;
typedef int16_t		rs16;
typedef int32_t		rs32;
typedef uint8_t		ru8; 
typedef uint16_t	ru16;
typedef uint32_t	ru32;

#define RA8876_SPI_CMDWRITE    0x00
#define RA8876_SPI_DATAWRITE   0x80
#define RA8876_SPI_DATAREAD    0xc0
#define RA8876_SPI_STATUSREAD  0x40
#define RA8876_SPI_CMDWRITE16    (ru16)0x0000
#define RA8876_SPI_DATAWRITE16   (ru16)0x8000
#define RA8876_SPI_DATAREAD16    (ru16)0xc000
#define RA8876_SPI_STATUSREAD16  (ru16)0x4000

/*==== [SW_(1)]  PLL  =====*/
//Crystal resonator for RA8876, suggested 10MHz
//SDRAMaccess clock,suggested 50~160MHz
//RA8876 system core clock, suggested 50~130MHz
//TFTdriving clock PCLK,refer to LCD SPEC specified PCLK frequency requirements 
//DRAM_FREQ>= CORE_FREQ   
//CORE_FREQ>= 2 * SCAN_FREQ

#define OSC_FREQ	10  // OSC clock frequency, unit: MHz.
#define DRAM_FREQ	120  // SDRAM clock frequency, unit: MHz. 
#define CORE_FREQ	120  // Core (system) clock frequency, unit: MHz. 
#define SCAN_FREQ	50  // Panel Scan clock frequency, unit: MHz.

/*==== [HW_(3)] SDRAM  =====*/
//#define IS42SM16160D
//#define IS42S16320B
//#define IS42S16400F
//#define M12L32162A
//#define M12L2561616A
//#define M12L64164A
//#define W9825G6JH
#define W9812G6JH
//#define MT48LC4M16A
//#define K4S641632N
//#define K4S281632K

/*TFT timing configure*/
#define TFT_MODE   0  //0:SYNC_mode(SYNC+DE mode), 1: DE mode  //if sync only mode do not connect DE signal or XDE_INV = 1
#define XHSYNC_INV  1 // 0:no inversion, 1:inversion 
#define XVSYNC_INV  1 // 0:no inversion, 1:inversion 
#define XDE_INV     0 // 0:no inversion, 1:inversion 
#define XPCLK_INV  1  // 0:no inversion, 1:inversion 
#define HPW       70   //
#define HND       160
#define HDW       1024
#define HST       160
#define VPW       10
#define VND       23
#define VDH       600
#define VST       12

#define RA8877_LVDS_FORMAT  0    // 0:Format1(VESA format), 1:Format2 =(JEIDA format) 

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576	// was 600, leave room for a status line

/*Page(image buffer) configure*/
/*The maximum number of pages is based on SDRAM capacity and color depth and width and height of one page*/
/*For example we used W9812G6JH SDRAM that capacity =  16Mbyte = 16777216 bytes*/
/*The SDRAM is divided into several image buffers and the maximum number of image buffers is limited by the 
memory size. For example : page_size = 1024*600*2byte(16bpp) = 1228800byte, maximum number = 16/1.2288 */
/*vertical multi page application*/
#define PAGE1_START_ADDR  0
#define PAGE2_START_ADDR  1024*600*2   // 1228800 bytes
#define PAGE3_START_ADDR  1024*600*2*2 // 2457600 
#define PAGE4_START_ADDR  1024*600*2*3
#define PAGE5_START_ADDR  1024*600*2*4
#define PAGE6_START_ADDR  1024*600*2*5
#define PAGE7_START_ADDR  1024*600*2*6
#define PAGE8_START_ADDR  1024*600*2*7
#define PAGE9_START_ADDR  1024*600*2*8
#define PAGE10_START_ADDR  1024*600*2*9

#define PATTERN1_RAM_START_ADDR 1024*600*2*10
#define PATTERN2_RAM_START_ADDR (1024*600*2*10)+(16*16*2)
#define PATTERN3_RAM_START_ADDR (1024*600*2*10)+(16*16*2)+(16*16*2)

/*DMA picture data start address*/
#define WP1_ADDR  0
#define WP2_ADDR  1024*600*2
#define WP3_ADDR  1024*600*2*2
#define WP4_ADDR  1024*600*2*3
#define WP5_ADDR  1024*600*2*4
#define WP6_ADDR  1024*600*2*5

#define COLOR65K_BLACK         0x0000
#define COLOR65K_WHITE         0xffff
#define COLOR65K_RED           0xf800
#define COLOR65K_LIGHTRED      0xfc10
#define COLOR65K_DARKRED       0x8000
#define COLOR65K_GREEN         0x07e0
#define COLOR65K_LIGHTGREEN    0x87f0
#define COLOR65K_DARKGREEN     0x0400
#define COLOR65K_BLUE          0x001f
#define COLOR65K_BLUE2         0x051f
#define COLOR65K_LIGHTBLUE     0x841f
#define COLOR65K_DARKBLUE      0x0010
#define COLOR65K_YELLOW        0xffe0
#define COLOR65K_LIGHTYELLOW   0xfff0
#define COLOR65K_DARKYELLOW    0x8400
#define COLOR65K_CYAN          0x07ff
#define COLOR65K_LIGHTCYAN     0x87ff
#define COLOR65K_DARKCYAN      0x0410
#define COLOR65K_MAGENTA       0xf81f
#define COLOR65K_LIGHTMAGENTA  0xfc1f
#define COLOR65K_DARKMAGENTA   0x8010
#define COLOR65K_BROWN         0xA145

#define COLOR65K_GRAYSCALE1    2113
#define COLOR65K_GRAYSCALE2    2113*2
#define COLOR65K_GRAYSCALE3    2113*3
#define COLOR65K_GRAYSCALE4    2113*4
#define COLOR65K_GRAYSCALE5    2113*5
#define COLOR65K_GRAYSCALE6    2113*6
#define COLOR65K_GRAYSCALE7    2113*7
#define COLOR65K_GRAYSCALE8    2113*8
#define COLOR65K_GRAYSCALE9    2113*9
#define COLOR65K_GRAYSCALE10   2113*10
#define COLOR65K_GRAYSCALE11   2113*11
#define COLOR65K_GRAYSCALE12   2113*12
#define COLOR65K_GRAYSCALE13   2113*13
#define COLOR65K_GRAYSCALE14   2113*14
#define COLOR65K_GRAYSCALE15   2113*15
#define COLOR65K_GRAYSCALE16   2113*16
#define COLOR65K_GRAYSCALE17   2113*17
#define COLOR65K_GRAYSCALE18   2113*18
#define COLOR65K_GRAYSCALE19   2113*19
#define COLOR65K_GRAYSCALE20   2113*20
#define COLOR65K_GRAYSCALE21   2113*21
#define COLOR65K_GRAYSCALE22   2113*22
#define COLOR65K_GRAYSCALE23   2113*23
#define COLOR65K_GRAYSCALE24   2113*24
#define COLOR65K_GRAYSCALE25   2113*25
#define COLOR65K_GRAYSCALE26   2113*26
#define COLOR65K_GRAYSCALE27   2113*27
#define COLOR65K_GRAYSCALE28   2113*28
#define COLOR65K_GRAYSCALE29   2113*29
#define COLOR65K_GRAYSCALE30   2113*30

#define STRING_LINE1  0
#define STRING_LINE2  32
#define STRING_LINE3  32*2
#define STRING_LINE4  32*3
#define STRING_LINE5  32*4
#define STRING_LINE6  32*5
#define STRING_LINE7  32*6
#define STRING_LINE8  32*7
#define STRING_LINE9  32*8
#define STRING_LINE10  32*9
#define STRING_LINE11  32*10
#define STRING_LINE12  32*11
#define STRING_LINE13  32*12
#define STRING_LINE14  32*13
#define STRING_LINE15  32*14
#define STRING_LINE16  32*15
#define STRING_LINE17  32*16
#define STRING_LINE18  32*17
#define STRING_LINE19  32*18

/*RA8876,8877 register & bit*/
#define RA8876_SRR  0x00
#define RA8876_SOFTWARE_RESET  0xD7

#define RA8876_CCR  0x01
#define RA8876_PLL_DISABLE  0  
#define RA8876_PLL_ENABLE   1
#define RA8876_WAIT_NO_MASK  0
#define RA8876_WAIT_MASK  1  
#define RA8876_KEY_SCAN_DISABLE  0
#define RA8876_KEY_SCAN_ENABLE  1
#define RA8876_TFT_OUTPUT24  0
#define RA8876_TFT_OUTPUT18  1
#define RA8876_TFT_OUTPUT16  3
#define RA8876_I2C_MASTER_DISABLE  0
#define RA8876_I2C_MASTER_ENABLE  1
#define RA8876_SERIAL_IF_DISABLE  0
#define RA8876_SERIAL_IF_ENABLE  1
#define RA8876_HOST_DATA_BUS_8BIT  0
#define RA8876_HOST_DATA_BUS_SERIAL  0
#define RA8876_HOST_DATA_BUS_16BIT  1

#define RA8876_MACR  0x02
#define RA8876_DIRECT_WRITE  0  // (for all 8 bits MPU I/F, 16 bits MPU I/F with 8bpp data mode 1 & 2, 16 bits MPU I/F with 16/24-bpp data mode 1 & serial host interface)
#define RA8876_MASK_HIGH_BYTE  2  // (ex. 16 bit MPU I/F with 8-bpp data mode 1)
#define RA8876_MASK_HIGH_BYTE_EVEN_DATA 3  //(ex. 16 bit MPU I/F with 24-bpp data mode 2)
#define RA8876_READ_MEMORY_LRTB  0
#define RA8876_READ_MEMORY_RLTB  1
#define RA8876_READ_MEMORY_TBLR  2
#define RA8876_READ_MEMORY_BTLR  3
#define RA8876_WRITE_MEMORY_LRTB  0
#define RA8876_WRITE_MEMORY_RLTB  1
#define RA8876_WRITE_MEMORY_TBLR  2
#define RA8876_WRITE_MEMORY_BTLR  3

#define RA8876_ICR  0x03
#define RA8876_OUTPUT_INT_LEVEL_LOW  0
#define RA8876_OUTPUT_INT_LEVEL_HIGH  1
#define RA8876_EXT_INT_WITHOUT_DEBOUNCE  0
#define RA8876_EXT_INT_WITH_DEBOUNCE  1
#define RA8876_EXT_INT_LOW_LEVEL_TRIGGER  0
#define RA8876_EXT_INT_FALLING_EDGE_TRIGGER  1
#define RA8876_EXT_INT_HIGH_LEVEL_TRIGGER  2
#define RA8876_EXT_INT_RISING_EDGE_TRIGGER  3
#define RA8876_GRAPHIC_MODE  0
#define RA8876_TEXT_MODE  1
#define RA8876_MEMORY_SELECT_IMAGE  0
#define RA8876_MEMORY_SELECT_GAMMA  1
#define RA8876_MEMORY_SELECT_CURSOR_RAM  2
#define RA8876_MEMORY_SELECT_PALETTE_RAM  3

#define RA8876_MRWDP  0x04

#define RA8876_PPLLC1  0x05
#define RA8876_PPLLC2  0x06
#define RA8876_MPLLC1  0x07
#define RA8876_MPLLC2  0x08
#define RA8876_SPLLC1  0x09
#define RA8876_SPLLC2  0x0A

#define RA8876_INTEN  0x0B
#define RA8876_INTF   0x0C
#define RA8876_MINTFR 0x0D
#define RA8876_PUENR  0x0E
#define RA8876_PSFSR  0x0F

#define RA8876_MPWCTR  0x10
#define RA8876_PIP1_WINDOW_DISABLE 0
#define RA8876_PIP1_WINDOW_ENABLE  1
#define RA8876_PIP2_WINDOW_DISABLE 0
#define RA8876_PIP2_WINDOW_ENABLE  1
#define RA8876_SELECT_CONFIG_PIP1  0
#define RA8876_SELECT_CONFIG_PIP2  1
#define RA8876_IMAGE_COLOCR_DEPTH_8BPP  0
#define RA8876_IMAGE_COLOCR_DEPTH_16BPP 1
#define RA8876_IMAGE_COLOCR_DEPTH_24BPP 2
#define RA8876_PANEL_SYNC_MODE 0
#define RA8876_PANEL_DE_MODE 1

#define RA8876_PIPCDEP  0x11
#define RA8876_PIP1_COLOR_DEPTH_8BPP  0
#define RA8876_PIP1_COLOR_DEPTH_16BPP 1
#define RA8876_PIP1_COLOR_DEPTH_24BPP 2
#define RA8876_PIP2_COLOR_DEPTH_8BPP  0
#define RA8876_PIP2_COLOR_DEPTH_16BPP  1
#define RA8876_PIP2_COLOR_DEPTH_24BPP  2

#define RA8876_DPCR  0x12
#define RA8876_PCLK_NO_INVERSION  0
#define RA8876_PCLK_INVERSION  1
#define RA8876_DISPLAY_OFF  0
#define RA8876_DISPLAY_ON  1
#define RA8876_COLOR_BAR_DISABLE  0
#define RA8876_COLOR_BAR_ENABLE  1
#define RA8876_VDIR_TB  0
#define RA8876_VDIR_BT  1
#define RA8876_OUTPUT_RGB  0
#define RA8876_OUTPUT_RBG  1
#define RA8876_OUTPUT_GRB  2
#define RA8876_OUTPUT_GBR  3
#define RA8876_OUTPUT_BRG  4
#define RA8876_OUTPUT_BGR  5
#define RA8876_OUTPUT_GRAY  6
#define RA8876_OUTPUT_IDLE  7

#define RA8876_PCSR  0x13
#define RA8876_XHSYNC_LOW_ACTIVE  0
#define RA8876_XHSYNC_HIGH_ACTIVE  1
#define RA8876_XVSYNC_LOW_ACTIVE  0
#define RA8876_XVSYNC_HIGH_ACTIVE  1
#define RA8876_XDE_HIGH_ACTIVE  0
#define RA8876_XDE_LOW_ACTIVE  1
#define RA8876_XDE_IDLE_LOW  0
#define RA8876_XDE_IDLE_HIGH  1
#define RA8876_XPCLK_IDLE_LOW  0
#define RA8876_XPCLK_IDLE_HIGH  1
#define RA8876_XPDAT_IDLE_LOW  0
#define RA8876_XPDAT_IDLE_HIGH  1
#define RA8876_XHSYNC_IDLE_LOW  0
#define RA8876_XHSYNC_IDLE_HIGH  1
#define RA8876_XVSYNC_IDLE_LOW  0
#define RA8876_XVSYNC_IDLE_HIGH  1

#define RA8876_HDWR    0x14
#define RA8876_HDWFTR  0x15
#define RA8876_HNDR    0x16
#define RA8876_HNDFTR  0x17
#define RA8876_HSTR    0x18
#define RA8876_HPWR    0x19

#define RA8876_VDHR0  0x1A
#define RA8876_VDHR1  0x1B
#define RA8876_VNDR0  0x1C
#define RA8876_VNDR1  0x1D
#define RA8876_VSTR   0x1E
#define RA8876_VPWR   0x1F

#define RA8876_MISA0   0x20
#define RA8876_MISA1   0x21
#define RA8876_MISA2   0x22
#define RA8876_MISA3   0x23

#define RA8876_MIW0    0x24
#define RA8876_MIW1    0x25
#define RA8876_MWULX0  0x26
#define RA8876_MWULX1  0x27
#define RA8876_MWULY0  0x28
#define RA8876_MWULY1  0x29

#define RA8876_PWDULX0  0x2A
#define RA8876_PWDULX1  0x2B
#define RA8876_PWDULY0  0x2C
#define RA8876_PWDULY1  0x2D
#define RA8876_PISA0    0x2E
#define RA8876_PISA1    0x2F
#define RA8876_PISA2    0x30
#define RA8876_PISA3    0x31
#define RA8876_PIW0     0x32
#define RA8876_PIW1     0x33
#define RA8876_PWIULX0  0x34
#define RA8876_PWIULX1  0x35
#define RA8876_PWIULY0  0x36
#define RA8876_PWIULY1  0x37
#define RA8876_PWW0     0x38
#define RA8876_PWW1     0x39
#define RA8876_PWH0     0x3A
#define RA8876_PWH1     0x3B

#define RA8876_GTCCR    0x3C
#define RA8876_BTCR     0x3D
#define RA8876_CURHS    0x3E
#define RA8876_CURVS    0x3F

#define RA8876_GCHP0    0x40
#define RA8876_GCHP1    0x41
#define RA8876_GCVP0    0x42
#define RA8876_GCVP1    0x43
#define RA8876_GCC0     0x44
#define RA8876_GCC1     0x45

#define RA8876_CVSSA0   0x50
#define RA8876_CVSSA1   0x51
#define RA8876_CVSSA2   0x52
#define RA8876_CVSSA3   0x53
#define RA8876_CVS_IMWTH0  0x54
#define RA8876_CVS_IMWTH1  0x55

#define RA8876_AWUL_X0  0x56
#define RA8876_AWUL_X1  0x57
#define RA8876_AWUL_Y0  0x58
#define RA8876_AWUL_Y1  0x59
#define RA8876_AW_WTH0  0x5A
#define RA8876_AW_WTH1  0x5B
#define RA8876_AW_HT0   0x5C
#define RA8876_AW_HT1   0x5D

#define RA8876_AW_COLOR  0x5E
#define RA8876_CANVAS_BLOCK_MODE  0
#define RA8876_CANVAS_LINEAR_MODE  1
#define RA8876_CANVAS_COLOR_DEPTH_8BPP  0
#define RA8876_CANVAS_COLOR_DEPTH_16BPP  1
#define RA8876_CANVAS_COLOR_DEPTH_24BPP  2

#define RA8876_CURH0  0x5F
#define RA8876_CURH1  0x60
#define RA8876_CURV0  0x61
#define RA8876_CURV1  0x62

#define RA8876_F_CURX0  0x63
#define RA8876_F_CURX1  0x64
#define RA8876_F_CURY0  0x65
#define RA8876_F_CURY1  0x66

#define RA8876_DCR0  0x67
#define RA8876_DRAW_LINE  0x80
#define RA8876_DRAW_TRIANGLE  0x82 
#define RA8876_DRAW_TRIANGLE_FILL  0xA2 

#define RA8876_DLHSR0  0x68
#define RA8876_DLHSR1  0x69
#define RA8876_DLVSR0  0x6A
#define RA8876_DLVSR1  0x6B
#define RA8876_DLHER0  0x6C
#define RA8876_DLHER1  0x6D
#define RA8876_DLVER0  0x6E
#define RA8876_DLVER1  0x6F
#define RA8876_DTPH0   0x70
#define RA8876_DTPH1   0x71
#define RA8876_DTPV0   0x72
#define RA8876_DTPV1   0x73

#define RA8876_DCR1   0x76
#define RA8876_DRAW_CIRCLE   0x80
#define RA8876_DRAW_CIRCLE_FILL  0xC0
#define RA8876_DRAW_ELLIPSE 0x80
#define RA8876_DRAW_ELLIPSE_FILL  0xC0
#define RA8876_DRAW_BOTTOM_LEFT_CURVE  0x90
#define RA8876_DRAW_BOTTOM_LEFT_CURVE_FILL  0xD0
#define RA8876_DRAW_UPPER_LEFT_CURVE  0x91
#define RA8876_DRAW_UPPER_LEFT_CURVE_FILL  0xD1
#define RA8876_DRAW_UPPER_RIGHT_CURVE  0x92
#define RA8876_DRAW_UPPER_RIGHT_CURVE_FILL  0xD2
#define RA8876_DRAW_BOTTOM_RIGHT_CURVE  0x93
#define RA8876_DRAW_BOTTOM_RIGHT_CURVE_FILL  0xD3
#define RA8876_DRAW_SQUARE  0xA0
#define RA8876_DRAW_SQUARE_FILL  0xE0
#define RA8876_DRAW_CIRCLE_SQUARE  0xB0
#define RA8876_DRAW_CIRCLE_SQUARE_FILL  0xF0

#define RA8876_ELL_A0  0x77
#define RA8876_ELL_A1  0x78
#define RA8876_ELL_B0  0x79
#define RA8876_ELL_B1  0x7A
#define RA8876_DEHR0   0x7B
#define RA8876_DEHR1   0x7C
#define RA8876_DEVR0   0x7D
#define RA8876_DEVR1   0x7E

#define RA8876_FGCR   0xD2
#define RA8876_FGCG   0xD3
#define RA8876_FGCB   0xD4

#define RA8876_PSCLR   0x84
#define RA8876_PRESCALER  0x03
#define RA8876_PMUXR  0x85
#define RA8876_PWM_TIMER_DIV1   0
#define RA8876_PWM_TIMER_DIV2   1
#define RA8876_PWM_TIMER_DIV4   2
#define RA8876_PWM_TIMER_DIV8   3
#define RA8876_XPWM1_OUTPUT_ERROR_FLAG   0
#define RA8876_XPWM1_OUTPUT_PWM_TIMER1   2
#define RA8876_XPWM1_OUTPUT_OSC_CLK   3
#define RA8876_XPWM0_GPIO_C7   0
#define RA8876_XPWM0_OUTPUT_PWM_TIMER0   2
#define RA8876_XPWM0_OUTPUT_CORE_CLK   3

#define RA8876_PCFGR   0x86
#define RA8876_PWM_TIMER1_INVERTER_OFF  0
#define RA8876_PWM_TIMER1_INVERTER_ON   1
#define RA8876_PWM_TIMER1_ONE_SHOT   0
#define RA8876_PWM_TIMER1_AUTO_RELOAD   1
#define RA8876_PWM_TIMER1_STOP  0
#define RA8876_PWM_TIMER1_START  1
#define RA8876_PWM_TIMER0_DEAD_ZONE_DISABLE   0
#define RA8876_PWM_TIMER0_DEAD_ZONE_ENABLE   1
#define RA8876_PWM_TIMER0_INVERTER_OFF   0
#define RA8876_PWM_TIMER0_INVERTER_ON   1
#define RA8876_PWM_TIMER0_ONE_SHOT   0
#define RA8876_PWM_TIMER0_AUTO_RELOAD   1
#define RA8876_PWM_TIMER0_STOP  0
#define RA8876_PWM_TIMER0_START  1

#define RA8876_DZ_LENGTH   0x87
#define RA8876_TCMPB0L     0x88
#define RA8876_TCMPB0H     0x89
#define RA8876_TCNTB0L     0x8A
#define RA8876_TCNTB0H     0x8B
#define RA8876_TCMPB1L     0x8C
#define RA8876_TCMPB1H     0x8D
#define RA8876_TCNTB1L     0x8E
#define RA8876_TCNTB1F     0x8F

#define RA8876_BTE_CTRL0   0x90
#define RA8876_BTE_ENABLE   1
#define RA8876_PATTERN_FORMAT8X8   0
#define RA8876_PATTERN_FORMAT16X16   1

#define RA8876_BTE_CTRL1  0x91
#define RA8876_BTE_ROP_CODE_0     0   //0 ( Blackness ) 
#define RA8876_BTE_ROP_CODE_1     1   //~S0・~S1 or ~ ( S0+S1 ) 
#define RA8876_BTE_ROP_CODE_2     2   //~S0・S1
#define RA8876_BTE_ROP_CODE_3     3   //~S0
#define RA8876_BTE_ROP_CODE_4     4   //S0・~S1
#define RA8876_BTE_ROP_CODE_5     5   //~S1
#define RA8876_BTE_ROP_CODE_6     6   //S0^S1
#define RA8876_BTE_ROP_CODE_7     7   //~S0+~S1 or ~ ( S0・S1 ) 
#define RA8876_BTE_ROP_CODE_8     8   //S0・S1
#define RA8876_BTE_ROP_CODE_9     9   //~ ( S0^S1 ) 
#define RA8876_BTE_ROP_CODE_10    10  //S1
#define RA8876_BTE_ROP_CODE_11    11  //~S0+S1
#define RA8876_BTE_ROP_CODE_12    12  //S0
#define RA8876_BTE_ROP_CODE_13    13  //S0+~S1
#define RA8876_BTE_ROP_CODE_14    14  //S0+S1
#define RA8876_BTE_ROP_CODE_15    15  //1 ( Whiteness ) 
#define RA8876_BTE_ROP_BUS_WIDTH8  7
#define RA8876_BTE_ROP_BUS_WIDTH16  15

#define RA8876_BTE_MPU_WRITE_WITH_ROP    0  
#define RA8876_BTE_MEMORY_COPY_WITH_ROP   2
#define RA8876_BTE_MEMORY_COPY_BACKWARDS_WITH_ROP   3
#define RA8876_BTE_MPU_WRITE_WITH_CHROMA   4
#define RA8876_BTE_MEMORY_COPY_WITH_CHROMA  5
#define RA8876_BTE_PATTERN_FILL_WITH_ROP  6
#define RA8876_BTE_PATTERN_FILL_WITH_CHROMA  7
#define RA8876_BTE_MPU_WRITE_COLOR_EXPANSION   8
#define RA8876_BTE_MPU_WRITE_COLOR_EXPANSION_WITH_CHROMA   9
#define RA8876_BTE_MEMORY_COPY_WITH_OPACITY  10
#define RA8876_BTE_MPU_WRITE_WITH_OPACITY  11
#define RA8876_BTE_SOLID_FILL  12
#define RA8876_BTE_MEMORY_COPY_WITH_COLOR_EXPANSION  14
#define RA8876_BTE_MEMORY_COPY_WITH_COLOR_EXPANSION_CHROMA  15

#define RA8876_BTE_COLR  0x92
#define RA8876_S0_COLOR_DEPTH_8BPP  0
#define RA8876_S0_COLOR_DEPTH_16BPP  1
#define RA8876_S0_COLOR_DEPTH_24BPP   2
#define RA8876_S1_COLOR_DEPTH_8BPP  0
#define RA8876_S1_COLOR_DEPTH_16BPP  1
#define RA8876_S1_COLOR_DEPTH_24BPP  2
#define RA8876_S1_CONSTANT_COLOR   3
#define RA8876_S1_8BIT_PIXEL_ALPHA_BLENDING  4
#define RA8876_S1_16BIT_PIXEL_ALPHA_BLENDING  5
#define RA8876_DESTINATION_COLOR_DEPTH_8BPP  0
#define RA8876_DESTINATION_COLOR_DEPTH_16BPP  1
#define RA8876_DESTINATION_COLOR_DEPTH_24BPP  2

#define RA8876_S0_STR0   0x93
#define RA8876_S0_STR1   0x94
#define RA8876_S0_STR2   0x95
#define RA8876_S0_STR3   0x96
#define RA8876_S0_WTH0   0x97
#define RA8876_S0_WTH1   0x98
#define RA8876_S0_X0     0x99
#define RA8876_S0_X1     0x9A
#define RA8876_S0_Y0     0x9B
#define RA8876_S0_Y1     0x9C
#define RA8876_S1_STR0   0x9D
#define RA8876_S1_STR1   0x9E
#define RA8876_S1_STR2   0x9F
#define RA8876_S1_RED    0x9D
#define RA8876_S1_GREEN  0x9E
#define RA8876_S1_BLUE   0x9F
#define RA8876_S1_STR3   0xA0
#define RA8876_S1_WTH0   0xA1
#define RA8876_S1_WTH1   0xA2

#define RA8876_S1_X0   0xA3
#define RA8876_S1_X1   0xA4
#define RA8876_S1_Y0   0xA5
#define RA8876_S1_Y1   0xA6
#define RA8876_DT_STR0  0xA7
#define RA8876_DT_STR1   0xA8
#define RA8876_DT_STR2   0xA9
#define RA8876_DT_STR3   0xAA
#define RA8876_DT_WTH0   0xAB
#define RA8876_DT_WTH1   0xAC
#define RA8876_DT_X0     0xAD
#define RA8876_DT_X1     0xAE
#define RA8876_DT_Y0     0xAF
#define RA8876_DT_Y1     0xB0
#define RA8876_BTE_WTH0  0xB1
#define RA8876_BTE_WTH1  0xB2
#define RA8876_BTE_HIG0  0xB3
#define RA8876_BTE_HIG1  0xB4
#define RA8876_APB_CTRL  0xB5

#define RA8876_DMA_CTRL  0xB6
#define RA8876_DMA_START  1
#define RA8876_SFL_CTRL  0xB7
#define RA8876_SERIAL_FLASH_SELECT0    0
#define RA8876_SERIAL_FLASH_SELECT1    1
#define RA8876_SERIAL_FLASH_FONT_MODE  0
#define RA8876_SERIAL_FLASH_DMA_MODE   1
#define RA8876_SERIAL_FLASH_ADDR_24BIT  0
#define RA8876_SERIAL_FLASH_ADDR_32BIT  1
#define RA8876_STANDARD_SPI_MODE  0
#define RA8876_FOLLOW_RA8876_MODE  1
#define RA8876_SPI_NORMAL_READ   0   //COMMAND 03h
#define RA8876_SPI_FAST_READ_8DUMMY   4   //COMMAND 0Bh
#define RA8876_SPI_FAST_READ_16DUMMY  8   //COMMAND 1Bh
#define RA8876_SPI_DUAL_READ_8DUMMY   2   //COMMAND 3Bh
#define RA8876_2XIO_READ_MODE_4DUMMY  3   //COMMAND BBh

#define RA8876_SPIDR  0xB8
#define RA8876_SPIMCR2   0xB9
#define RA8876_SPIM_INT_DISABLE   0
#define RA8876_SPIM_INT_ENABLE    1
#define RA8876_SPIM_NSS_SELECT_0  0
#define RA8876_SPIM_NSS_SELECT_1  1
#define RA8876_SPIM_NSS_INACTIVE  0  //nSS port will goes high
#define RA8876_SPIM_NSS_ACTIVE    1  //nSS port will goes low
#define RA8876_SPIM_OVFIRQMSK_UNMASK  0
#define RA8876_SPIM_OVFIRQMSK_MASK    1 
#define RA8876_SPIM_EMTIRQMSK_UNMASK  0
#define RA8876_SPIM_EMTIRQMSK_MASK    1
#define RA8876_SPIM_MODE0   0
#define RA8876_SPIM_MODE1   1
#define RA8876_SPIM_MODE2   2
#define RA8876_SPIM_MODE3   3

#define RA8876_SPIMSR  0xBA

#define RA8876_SPI_DIVSOR 0xBB
#define RA8876_SPI_DIV2   0
#define RA8876_SPI_DIV4   1
#define RA8876_SPI_DIV6   2
#define RA8876_SPI_DIV8   3
#define RA8876_SPI_DIV10  4

#define RA8876_DMA_SSTR0  0xBC
#define RA8876_DMA_SSTR1  0xBD
#define RA8876_DMA_SSTR2  0xBE
#define RA8876_DMA_SSTR3  0xBF

#define RA8876_DMA_DX0  0xC0
#define RA8876_DMA_DX1  0xC1
#define RA8876_DMA_DY0  0xC2
#define RA8876_DMA_DY1  0xC3

#define RA8876_DMAW_WTH0   0xC6
#define RA8876_DMAW_WTH1   0xC7
#define RA8876_DMAW_HIGH0  0xC8
#define RA8876_DMAW_HIGH1  0xC9
#define RA8876_DMA_SWTH0   0xCA
#define RA8876_DMA_SWTH1   0xCB

#define RA8876_CCR0   0xCC
#define RA8876_SELECT_INTERNAL_CGROM  0
#define RA8876_SELECT_EXTERNAL_CGROM  1  //Genitop serial flash
#define RA8876_SELECT_USER_DEFINED  2
#define RA8876_CHAR_HEIGHT_16  0
#define RA8876_CHAR_HEIGHT_24  1
#define RA8876_CHAR_HEIGHT_32  2
#define RA8876_SELECT_8859_1   0
#define RA8876_SELECT_8859_2   1
#define RA8876_SELECT_8859_4   2
#define RA8876_SELECT_8859_5   3

#define RA8876_CCR1   0xCD
#define RA8876_TEXT_FULL_ALIGN_DISABLE   0
#define RA8876_TEXT_FULL_ALIGN_ENABLE    1
#define RA8876_TEXT_CHROMA_KEY_DISABLE   0
#define RA8876_TEXT_CHROMA_KEY_ENABLE    1
#define RA8876_TEXT_NO_ROTATION  0
#define RA8876_TEXT_ROTATION     1  
#define RA8876_TEXT_WIDTH_ENLARGEMENT_X1  0
#define RA8876_TEXT_WIDTH_ENLARGEMENT_X2  1
#define RA8876_TEXT_WIDTH_ENLARGEMENT_X3  2
#define RA8876_TEXT_WIDTH_ENLARGEMENT_X4  3
#define RA8876_TEXT_HEIGHT_ENLARGEMENT_X1  0
#define RA8876_TEXT_HEIGHT_ENLARGEMENT_X2  1
#define RA8876_TEXT_HEIGHT_ENLARGEMENT_X3  2
#define RA8876_TEXT_HEIGHT_ENLARGEMENT_X4  3

#define RA8876_GTFNT_SEL   0xCE
#define RA8876_GT21L16T1W   0
#define RA8876_GT30L16U2W   1
#define RA8876_GT30L24T3Y   2
#define RA8876_GT30L24M1Z   3
#define RA8876_GT30L32S4W   4
#define RA8876_GT20L24F6Y   5
#define RA8876_GT21L24S1W   6

#define RA8876_GTFNT_CR   0xCF
#define RA8876_GB2312             0
#define RA8876_GB12345_GB18030    1
#define RA8876_BIG5               2
#define RA8876_ASCII              4
#define RA8876_UNICODE            3
#define RA8876_UNI_JAPANESE       5
#define RA8876_JIS0208            6
#define RA8876_LATIN_GREEK_CYRILLIC_ARABIC_THAI_HEBREW   7
#define RA8876_ISO_8859_1_AND_ASCII   17
#define RA8876_ISO_8859_2_AND_ASCII   18
#define RA8876_ISO_8859_3_AND_ASCII   19
#define RA8876_ISO_8859_4_AND_ASCII   20
#define RA8876_ISO_8859_5_AND_ASCII   21
#define RA8876_ISO_8859_7_AND_ASCII   22
#define RA8876_ISO_8859_8_AND_ASCII   23
#define RA8876_ISO_8859_9_AND_ASCII   24
#define RA8876_ISO_8859_10_AND_ASCII  25
#define RA8876_ISO_8859_11_AND_ASCII  26
#define RA8876_ISO_8859_13_AND_ASCII  27
#define RA8876_ISO_8859_14_AND_ASCII  28
#define RA8876_ISO_8859_15_AND_ASCII  29
#define RA8876_ISO_8859_16_AND_ASCII  30
#define RA8876_GT_FIXED_WIDTH  0
#define RA8876_GT_VARIABLE_WIDTH_ARIAL  1
#define RA8876_GT_VARIABLE_FIXED_WIDTH_ROMAN  2
#define RA8876_GT_BOLD  3

#define RA8876_FLDR     0xD0
#define RA8876_F2FSSR   0xD1
#define RA8876_FGCR     0xD2
#define RA8876_FGCG     0xD3
#define RA8876_FGCB     0xD4
#define RA8876_BGCR     0xD5
#define RA8876_BGCG     0xD6
#define RA8876_BGCB     0xD7

#define RA8876_CGRAM_STR0   0xDB
#define RA8876_CGRAM_STR1   0xDC
#define RA8876_CGRAM_STR2   0xDD
#define RA8876_CGRAM_STR3   0xDE

#define RA8876_PMU   0xDF
#define RA8876_POWER_NORMAL_STATE  0x00
#define RA8876_WAKE_UP       0x00
#define RA8876_STANDBY_MODE  0x81
#define RA8876_SUSPEND_MODE  0x82
#define RA8876_SLEEP_MODE    0x83

#define RA8876_SDRAR   0xE0
#define RA8876_SDRAM_POWER_DOWN   0
#define RA8876_SDRAM_SELF_REFRESH   1
#define RA8876_SDRAM_SDR   0
#define RA8876_SDRAM_MOBILE   1
#define RA8876_SDRAM_2BANKS   0
#define RA8876_SDRAM_4BANKS   1
#define RA8876_SDRAM_ROW_ADDR_2K  0 
#define RA8876_SDRAM_ROW_ADDR_4K  1
#define RA8876_SDRAM_ROW_ADDR_8K  2
#define RA8876_SDRAM_COLUMN_256  0
#define RA8876_SDRAM_COLUMN_512  1
#define RA8876_SDRAM_COLUMN_1024  2
#define RA8876_SDRAM_COLUMN_2048  3
#define RA8876_SDRAM_COLUMN_4096  4

#define RA8876_SDRMD   0xE1
#define RA8876_SDRAM_FULL_ARRAY   0
#define RA8876_SDRAM_HALF_ARRAY   1
#define RA8876_SDRAM_QUARTER_ARRAY   2
#define RA8876_SDRAM_ONE_EIGHTH_ARRAY   5
#define RA8876_SDRAM_ONE_SIXTEENTH_ARRAY   6
#define RA8876_SDRAM_FULL_STRENGTH_DRIVER   0
#define RA8876_SDRAM_HALF_STRENGTH_DRIVER   1
#define RA8876_SDRAM_QUARTER_STRENGTH_DRIVER   2
#define RA8876_SDRAM_ONE_EIGHT_STRENGTH_DRIVER  3
#define RA8876_SDRAM_CAS_LATENCY_2   2
#define RA8876_SDRAM_CAS_LATENCY_3   3

#define RA8876_SDR_REF_ITVL0   0xE2
#define RA8876_SDR_REF_ITVL1    0xE3

#define RA8876_SDRCR   0xE4
#define RA8876_SDRAM_BREAK_LEN_256   0
#define RA8876_SDRAM_BREAK_LEN_128   1
#define RA8876_SDRAM_BREAK_LEN_64    2
#define RA8876_SDRAM_BREAK_LEN_32    3
#define RA8876_SDRAM_BUS_WIDTH_16    0
#define RA8876_SDRAM_BUS_WIDTH_32    1
#define RA8876_SDRAM_XMCKE_DISABLE   0
#define RA8876_SDRAM_XMCKE_ENABLE    1
#define RA8876_SDRAM_DISABLE_WARNING  0
#define RA8876_SDRAM_ENABLE_WARNING   1
#define RA8876_SDRAM_TIMING_PARA_DISABLE   0
#define RA8876_SDRAM_TIMING_PARA_ENABLE   1
#define RA8876_SDRAM_ENTER_POWER_SAVING   1 //0 to 1 transition will enter power saving mode
#define RA8876_SDRAM_EXIT_POWER_SAVING   0  //1 to 0 transition will exit power saving mode
#define RA8876_SDRAM_INITIALIZE   1  // An 0 to 1 transition will execute SDRAM initialization procedure.
#define RA8876_SDRAM_PARAMETER1   0xE0
#define RA8876_SDRAM_PARAMETER2   0xE1
#define RA8876_SDRAM_PARAMETER3   0xE2
#define RA8876_SDRAM_PARAMETER4   0xE3

#define RA8876_I2CMCPR0   0xE5
#define RA8876_I2CMCPR1   0xE6
#define RA8876_I2CMTXR   0xE7
#define RA8876_I2CMRXR   0xE8
#define RA8876_I2CMCMDR   0xE9
#define RA8876_I2CMSTUR   0xEA

#define RA8876_GPIOAD   0xF0
#define RA8876_GPIOA    0xF1
#define RA8876_GPIOB    0xF2
#define RA8876_GPIOCD   0xF3
#define RA8876_GPIOC    0xF4
#define RA8876_GPIODD   0xF5
#define RA8876_GPIOD    0xF6
#define RA8876_GPIOED   0xF7
#define RA8876_GPIOE    0xF8
#define RA8876_GPIOFD   0xF9
#define RA8876_GPIOF    0xFA

#define RA8876_KSCR1    0xFB
#define RA8876_KSCR2    0xFC
#define RA8876_KSDR0    0xFD
#define RA8876_KSDR1    0xFE
#define RA8876_KSDR2    0xFF

// BIT SET/RESET Defs.
#define	cSetb0		0x01
#define	cSetb1		0x02
#define	cSetb2		0x04
#define	cSetb3		0x08
#define	cSetb4		0x10
#define	cSetb5		0x20
#define	cSetb6		0x40
#define	cSetb7		0x80

#define	cClrb0		0xfe
#define	cClrb1		0xfd
#define	cClrb2		0xfb
#define	cClrb3		0xf7
#define	cClrb4		0xef
#define	cClrb5		0xdf
#define	cClrb6		0xbf
#define	cClrb7		0x7f

static volatile ru32 _maxspeed = 5000000; // Default to a relatively slow speed for breadboard testing
					   //A "good" PCB can get up to 75000000 (75MHz) with a Teensy 4

const uint32_t MEM_SIZE_MAX	= 16l*1024l*1024l;	///Max. size in byte of SDRAM

class Ra8876_Lite : public Print
{
private:
	// int _xnscs, _xnreset;
	int _mosi;
	int _miso;
	int _sclk;
	int _cs;
	int _rst;
	int	_errorCode;

public:
	// Global Variables
	volatile boolean			  _textMode;
	int16_t 		 			  _width, 			  _height;
	int16_t						  _cursorX, 		  _cursorY;
	uint8_t						  _scaleX,			  _scaleY;
	uint8_t						  _FNTwidth, 		  _FNTheight;
	uint8_t _fontheight;
	uint8_t _cursorXsize;
	uint8_t _cursorYsize;

	volatile bool	RA8876_BUSY; //This is used to show an SPI transaction is in progress. 

	uint32_t currentPage;
	uint32_t lastPage;
	uint32_t pageOffset;
	uint8_t currentFont;

	// Text Sreen Vars
	uint8_t	vdata;
	uint8_t leftmarg;
	uint8_t topmarg;
	uint8_t rightmarg;
	uint8_t bottommarg;
	uint8_t tab_size;
	uint16_t prompt_size; // prompt ">"
	uint16_t prompt_line; // current text prompt row
	uint16_t CharPosX, CharPosY;
	boolean UDFont;

	//scroll vars ----------------------------
	uint16_t	_scrollXL,_scrollXR,_scrollYT,_scrollYB;
	uint16_t	_TXTForeColor;
	uint16_t	_TXTBackColor;


	Ra8876_Lite(int CSp, int RSTp, int mosi_pin, int sclk_pin, int miso_pin);
	/* Initialize RA8876 */
	boolean Ra8876_begin(void);
	boolean ra8876Initialize(void); 
	boolean ra8876PllInitial (void);
	boolean ra8876SdramInitial(void);

	/*access*/
	void lcdRegWrite(ru8 reg, bool finalize = true);
	void lcdDataWrite(ru8 data, bool finalize = true);
	ru8 lcdDataRead(bool finalize = true);
	ru16 lcdDataRead16bpp(bool finalize = true);
	ru8 lcdStatusRead(bool finalize = true);
	void lcdRegDataWrite(ru8 reg,ru8 data, bool finalize = true);
	ru8 lcdRegDataRead(ru8 reg, bool finalize = true);
	void lcdDataWrite16bbp(ru16 data, bool finalize = true); 
	/*Status*/
	void checkWriteFifoNotFull(void);
	void checkWriteFifoEmpty(void);
	void checkReadFifoNotFull(void);
	void checkReadFifoFull(void);
	void checkReadFifoNotEmpty(void);
	void check2dBusy(void);
	boolean checkSdramReady(void);
	ru8 powerSavingStatus(void);
	boolean checkIcReady(void);//

	void displayOn(boolean on);
	void lcdHorizontalWidthVerticalHeight(ru16 width,ru16 height);
	void lcdHorizontalNonDisplay(ru16 numbers);
	void lcdHsyncStartPosition(ru16 numbers);
	void lcdHsyncPulseWidth(ru16 numbers);
	void lcdVerticalNonDisplay(ru16 numbers);
	void lcdVsyncStartPosition(ru16 numbers);
	void lcdVsyncPulseWidth(ru16 numbers);
	void displayImageStartAddress(ru32 addr);
	void displayImageWidth(ru16 width);
	void displayWindowStartXY(ru16 x0,ru16 y0);
	void canvasImageStartAddress(ru32 addr);
	void canvasImageWidth(ru16 width);
	void activeWindowXY(ru16 x0,ru16 y0);
	void activeWindowWH(ru16 width,ru16 height);

	void bte_Source0_MemoryStartAddr(ru32 addr);
	void bte_Source0_ImageWidth(ru16 width);
	void bte_Source0_WindowStartXY(ru16 x0,ru16 y0);
	void bte_Source1_MemoryStartAddr(ru32 addr);
	void bte_Source1_ImageWidth(ru16 width);
	void bte_Source1_WindowStartXY(ru16 x0,ru16 y0);
	void bte_DestinationMemoryStartAddr(ru32 addr);
	void bte_DestinationImageWidth(ru16 width);
	void bte_DestinationWindowStartXY(ru16 x0,ru16 y0);
	void bte_WindowSize(ru16 width, ru16 height);

	/*PWM function*/
	void pwm_Prescaler(ru8 prescaler);
	void pwm_ClockMuxReg(ru8 pwm1_clk_div, ru8 pwm0_clk_div, ru8 xpwm1_ctrl, ru8 xpwm0_ctrl);
	void pwm_Configuration(ru8 pwm1_inverter,ru8 pwm1_auto_reload,ru8 pwm1_start,ru8 
						  pwm0_dead_zone, ru8 pwm0_inverter, ru8 pwm0_auto_reload,ru8 pwm0_start);

	void pwm0_ClocksPerPeriod(ru16 clocks_per_period);
	void pwm0_Duty(ru16 duty);
	void pwm1_ClocksPerPeriod(ru16 clocks_per_period);
	void pwm1_Duty(ru16 duty);
			
	void ramAccessPrepare(void);
	void foreGroundColor16bpp(ru16 color, bool finalize = true);
	void backGroundColor16bpp(ru16 color, bool finalize = true);

	/*graphic function*/
	void graphicMode(boolean on);
	void setPixelCursor(ru16 x,ru16 y);
	void drawPixel(ru16 x, ru16 y, ru16 color);
	ru16 getPixel(ru16 x, ru16 y);
	void putPicture_16bpp(ru16 x,ru16 y,ru16 width, ru16 height);
	void putPicture_16bppData8(ru16 x,ru16 y,ru16 width, ru16 height, const unsigned char *data);
	void putPicture_16bppData16(ru16 x,ru16 y,ru16 width, ru16 height, const unsigned short *data);
	void Memory_Select_SDRAM(void);
	void Memory_Select_Graphic_Cursor_RAM(void);
	void Enable_Graphic_Cursor(void);
	void Disable_Graphic_Cursor(void);
	void Select_Graphic_Cursor_1(void);
	void Select_Graphic_Cursor_2(void);
	void Select_Graphic_Cursor_3(void);
	void Select_Graphic_Cursor_4(void);
	void Memory_Select_CGRAM(void);
	void CGRAM_initial(uint32_t charAddr, const uint8_t *data, uint16_t count);
	void Memory_XY_Mode(void);
	void Memory_Linear_Mode(void);
	//**[40h][41h][42h][43h]**//
	void Graphic_Cursor_XY(unsigned short WX,unsigned short HY);
	//**[44]**//
	void Set_Graphic_Cursor_Color_1(unsigned char temp);
	//**[45]**//
	void Set_Graphic_Cursor_Color_2(unsigned char temp);
	void Graphic_cursor_initial(void);

	uint32_t boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);
	uint32_t boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);

	void initButton(struct Gbuttons *button, uint16_t x, uint16_t y, uint8_t w, uint8_t h,
	 uint16_t outline, uint16_t fill, uint16_t textcolor,
	 char *label, uint8_t textsize);

	void drawButton(struct Gbuttons *buttons, bool inverted);
	bool buttonContains(struct Gbuttons *buttons,uint16_t x, uint16_t y);
	void buttonPress(struct Gbuttons *buttons, bool p);
	bool buttonIsPressed(struct Gbuttons *buttons);
	bool buttonJustPressed(struct Gbuttons *buttons);
	bool buttonJustReleased(struct Gbuttons *buttons);
	void Color_Bar_ON(void);
	void Color_Bar_OFF(void);

	/*text function*/
	void textMode(boolean on);
	void textColor(ru16 foreground_color,ru16 background_color);
	void setTextCursor(ru16 x,ru16 y);
	void textxy(ru16 x, ru16 y);
	void buildTextScreen(void);
	void setFontSource(uint8_t source);
	//**[5Fh]~[62h]**//
	void linearAddressSet(ru32 addr);

	ru8 vmemReadData(ru32 addr);
	ru16 vmemReadData16(ru32 addr);
	void vmemWriteData(ru32 addr, ru8 vmemData);
	void vmemWriteData16(ru32 addr, ru16 vmemData);

	//**[DBh]~[DEh]**//
	void CGRAM_Start_address(uint32_t Addr);
	void setTextParameter1(ru8 source_select,ru8 size_select,ru8 iso_select);//cch
	void setTextParameter2(ru8 align, ru8 chroma_key, ru8 width_enlarge, ru8 height_enlarge);//cdh
	void genitopCharacterRomParameter(ru8 scs_select, ru8 clk_div, ru8 rom_select, ru8 character_select, ru8 gt_width);//b7h,bbh,ceh,cfh

	void clearActiveScreen(void);
	void clreol(void);
	void clreos(void);
	void clrbol(void);
	void clrbos(void);
	void clrlin(void);
	void clearStatusLine(uint16_t color); 
	void putString(ru16 x0,ru16 y0, const char *str);
	void writeStatusLine(ru16 x0, uint16_t fgcolor, uint16_t bgcolor, const char *str);

	// overwrite print functions:
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	
	size_t rawPrint(uint8_t text);
	void update_xy(void);
	void update_tft(uint8_t data);
	void Enable_Text_Cursor(void);
	void Disable_Text_Cursor(void);
	void Enable_Text_Cursor_Blinking(void);
	void Disable_Text_Cursor_Blinking(void);
	void Blinking_Time_Frames(unsigned char temp);
	void Text_Cursor_H_V(unsigned short WX,unsigned short HY);
	void scroll(void);
	void scrollDown(void);
	 
	/*draw function*/
	void drawLine(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void drawSquare(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color);
	void drawSquareFill(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color);
	void drawCircleSquare(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 xr, ru16 yr, ru16 color);
	void drawCircleSquareFill(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 xr, ru16 yr, ru16 color);
	void drawTriangle(ru16 x0,ru16 y0,ru16 x1,ru16 y1,ru16 x2,ru16 y2,ru16 color);
	void drawTriangleFill(ru16 x0,ru16 y0,ru16 x1,ru16 y1,ru16 x2,ru16 y2,ru16 color);
	void drawCircle(ru16 x0,ru16 y0,ru16 r,ru16 color);
	void drawCircleFill(ru16 x0,ru16 y0,ru16 r,ru16 color);
	void drawEllipse(ru16 x0,ru16 y0,ru16 xr,ru16 yr,ru16 color);
	void drawEllipseFill(ru16 x0,ru16 y0,ru16 xr,ru16 yr,ru16 color);
	 
	/*BTE function*/
	void bteMemoryCopy(ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,ru32 des_addr,ru16 des_image_width, 
					   ru16 des_x,ru16 des_y,ru16 copy_width,ru16 copy_height);
	void bteMemoryCopyWithROP(ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,
							   ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 copy_width,ru16 copy_height,ru8 rop_code);
	void bteMemoryCopyWithChromaKey(ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
								   ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 copy_width,ru16 copy_height,ru16 chromakey_color);
	void bteMpuWriteWithROPData8(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,
							ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code,const unsigned char *data);
	void bteMpuWriteWithROPData16(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,
							ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code,const unsigned short *data);
	void bteMpuWriteWithROP(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,
							ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code);                     
	void bteMpuWriteWithChromaKeyData8(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color,
								 const unsigned char *data);
	void bteMpuWriteWithChromaKeyData16(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color,
								 const unsigned short *data);
	void bteMpuWriteWithChromaKey(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color);
	void bteMpuWriteColorExpansionData(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color,const unsigned char *data);
	void bteMpuWriteColorExpansion(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color);
	void bteMpuWriteColorExpansionWithChromaKeyData(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,
												ru16 foreground_color,ru16 background_color,const unsigned char *data);
	void bteMpuWriteColorExpansionWithChromaKey(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,
												ru16 width,ru16 height,ru16 foreground_color,ru16 background_color);

	void btePatternFill(ru8 p8x8or16x16, ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
					   ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height);
	void btePatternFillWithChromaKey(ru8 p8x8or16x16, ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
									ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color);
	/*DMA function*/
	void setSerialFlash4BytesMode(ru8 scs_select);
	void dma_24bitAddressBlockMode(ru8 scs_selct,ru8 clk_div,ru16 x0,ru16 y0,ru16 width,ru16 height,ru16 picture_width,ru32 addr);
	void dma_32bitAddressBlockMode(ru8 scs_selct,ru8 clk_div,ru16 x0,ru16 y0,ru16 width,ru16 height,ru16 picture_width,ru32 addr);


	//SPI Functions
	inline __attribute__((always_inline)) 
	void startSend(){
		if(!RA8876_BUSY) {
	        RA8876_BUSY = true;
			SPI.beginTransaction(SPISettings(_maxspeed, MSBFIRST, SPI_MODE0));
		}
	    digitalWriteFast(_cs, LOW);
	}

	inline __attribute__((always_inline)) 
	void endSend(bool finalize){
		digitalWriteFast(_cs, HIGH);
		if(finalize) {
			SPI.endTransaction();
			RA8876_BUSY = false;
		}
	} 

	/* PIP window funtions */
	void PIP
	(
	 unsigned char On_Off // 0 : disable PIP, 1 : enable PIP, 2 : To maintain the original state
	,unsigned char Select_PIP // 1 : use PIP1 , 2 : use PIP2
	,unsigned long PAddr //start address of PIP
	,unsigned short XP //coordinate X of PIP Window, It must be divided by 4.
	,unsigned short YP //coordinate Y of PIP Window, It must be divided by 4.
	,unsigned long ImageWidth //Image Width of PIP (recommend = canvas image width)
	,unsigned short X_Dis //coordinate X of Display Window
	,unsigned short Y_Dis //coordinate Y of Display Window
	,unsigned short X_W //width of PIP and Display Window, It must be divided by 4.
	,unsigned short Y_H //height of PIP and Display Window , It must be divided by 4.
	);

	void PIP_Display_Start_XY(unsigned short WX,unsigned short HY);	
	void PIP_Image_Start_Address(unsigned long Addr);	
	void PIP_Image_Width(unsigned short WX);	
	void PIP_Window_Image_Start_XY(unsigned short WX,unsigned short HY);	
	void PIP_Window_Width_Height(unsigned short WX,unsigned short HY);	

	//**[10h]**//
	void Enable_PIP1(void);
	void Disable_PIP1(void);
	void Enable_PIP2(void);
	void Disable_PIP2(void);
	void Select_PIP1_Parameter(void);
	void Select_PIP2_Parameter(void);
	void Select_Main_Window_8bpp(void);
	void Select_Main_Window_16bpp(void);
	void Select_Main_Window_24bpp(void);
	void Select_LCD_Sync_Mode(void);
	void Select_LCD_DE_Mode(void);
	//**[11h]**//
	void Select_PIP1_Window_8bpp(void);
	void Select_PIP1_Window_16bpp(void);
	void Select_PIP1_Window_24bpp(void);
	void Select_PIP2_Window_8bpp(void);
	void Select_PIP2_Window_16bpp(void);
	void Select_PIP2_Window_24bpp(void);

};

#endif
