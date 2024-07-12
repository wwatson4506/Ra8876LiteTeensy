//**************************************************************//
// Teensy 4.1 8080 Parallel 8/16 bit with 8 bit ASYNC support.
//**************************************************************//
/*
 * Ra8876LiteTeensy.cpp
 * Modified Version of: File Name : RA8876_t41_p.cpp
 *			Author    : RAiO Application Team
 *			Edit Date : 09/13/2017
 *			Version   : v2.0  1.modify bte_DestinationMemoryStartAddr bug
 *                 			  2.modify ra8876SdramInitial Auto_Refresh
 *                 			  3.modify ra8876PllInitial
 ****************************************************************
 * 	  	              : New 8080 Parallel version
 *                    : For MicroMod
 *                    : By Warren Watson
 *                    : 06/07/2018 - 05/03/2024
 *                    : Copyright (c) 2017-2024 Warren Watson.
 *****************************************************************
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
/*File Name : tft.h
 *          : For Teensy 3.x and T4
 *          : By Warren Watson
 *          : 06/07/2018 - 11/31/2019
 *          : Copyright (c) 2017-2019 Warren Watson.
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
/***************************************************************
 *  Modified by mjs513 and KurtE and MorganS
 *  Combined libraries and added functions to be compatible with
 *  other display libraries
 *  See PJRC Forum Thread: https://forum.pjrc.com/threads/58565-RA8876LiteTeensy-For-Teensy-T36-and-T40/page5
 *
 ***************************************************************/

#include "Arduino.h"
#include "RA8876Registers.h"
#include "RA8876_common.h"

#ifndef _RA8876_T41_P
#define _RA8876_T41_P

// #define WINT 33       // RA8876 xnWAIT pin (Dev Board)

#include "FlexIO_t4.h"

#define WR_PIN 36
#define RD_PIN 37

// #define RA8876_CLOCK_READ 30   //equates to 8mhz
#define RA8876_CLOCK_READ 60 // equates to 4mhz
//#define RA8876_CLOCK_READ 120   //equates to 2mhz



//#define BUS_WIDTH 8 /*Available options are 8 or 16 */
#define SHIFTNUM 8  // number of shifters used (up to 8)
#define BYTES_PER_BEAT (sizeof(uint8_t))
#define BEATS_PER_SHIFTER (sizeof(uint32_t) / BYTES_PER_BEAT)
#define BYTES_PER_BURST (sizeof(uint32_t) * SHIFTNUM)
#define SHIFTER_IRQ (SHIFTNUM - 1)
#define TIMER_IRQ 0
#define FLEXIO_ISR_PRIORITY 64 // interrupt is timing sensitive, so use relatively high priority (supersedes USB)
#define use_lcdDataWrite16bbp

class RA8876_t41_p : public RA8876_common {
  public:
    RA8876_t41_p(const uint8_t DCp = 13, const uint8_t CSp = 11, const uint8_t RSTp = 12);

    /* Initialize RA8876 */
    boolean begin(uint8_t baud_div);

    // If used this must be called before begin
    // Set the FlexIO pins.  The first version you can specify just the wr, and read and optionsl first Data.
    // it will use information in the Flexio library to fill in d1-d7
    bool setFlexIOPins(uint8_t write_pin, uint8_t rd_pin, uint8_t tft_d0 = 0xff);

    // Set the FlexIO pins.  Specify all of the pins for 8 (or 16) bit mode. Must be called before begin
    bool setFlexIOPins(uint8_t write_pin, uint8_t rd_pin, 
                      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                      uint8_t d8=0xff, uint8_t d9=0xff, uint8_t d10=0xff, uint8_t d11=0xff,
                      uint8_t d12=0xff, uint8_t d13=0xff, uint8_t d14=0xff, uint8_t d15=0xff
                      );




    /*access*/
    void lcdRegWrite(ru8 reg, bool finalize = true);
    void lcdDataWrite(ru8 data, bool finalize = true);
    ru8 lcdDataRead(bool finalize = true);
    ru8 lcdStatusRead(bool finalize = true);
    void lcdRegDataWrite(ru8 reg, ru8 data, bool finalize = true);
    ru8 lcdRegDataRead(ru8 reg, bool finalize = true);
    void lcdDataWrite16bbp(ru16 data, bool finalize = true);
    void lcdDataWrite16(uint16_t data, bool finalize = true);
    void LCD_CmdWrite(unsigned char cmd);

    /*BTE function*/
    void bteMpuWriteWithROPData8(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width,
                                 ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code, const unsigned char *data);
    void bteMpuWriteWithROPData16(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width,
                                  ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code, const unsigned short *data);
    void bteMpuWriteWithROP(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width,
                            ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code);

    void bteMpuWriteWithChromaKeyData8(ru32 des_addr, ru16 des_image_width, ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru16 chromakey_color,
                                       const unsigned char *data);

    void bteMpuWriteWithChromaKeyData16(ru32 des_addr, ru16 des_image_width, ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru16 chromakey_color,
                                        const unsigned short *data);

    /*  Picture Functions */
	void putPicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *data);
    void putPicture_16bpp(ru16 x, ru16 y, ru16 width, ru16 height);                                   // not recommended: use BTE instead
    void putPicture_16bppData8(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned char *data);   // not recommended: use BTE instead
    void putPicture_16bppData16(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned short *data); // not recommended: use BTE instead

    // helper functions for some of the graphic primitives.
    void beginWrite16BitColors();
    void write16BitColor(uint16_t color);
    void endWrite16BitColors();

    inline __attribute__((always_inline)) void endSend(bool finalize) {
        // dummy function
    }

    typedef void (*CBF)();
    CBF _callback;
    void onCompleteCB(CBF callback) {_callback = callback; }

    void FlexIO_Clear_Config_SnglBeat();
    void MulBeatWR_nPrm_IRQ(const void *value, uint32_t const length);
    void pushPixels16bitAsync(const uint16_t *pcolors, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void lcdRegDataWrite16(ru8 reg, ru16 data, bool finalize);

    FlexIOHandler *pFlex;
    IMXRT_FLEXIO_t *p;
    const FlexIOHandler::FLEXIO_Hardware_t *hw;

    uint8_t _baud_div = 20;
    int8_t _dc; //, _cs, _rst;

    uint8_t _dummy;

    uint32_t MulBeatCountRemain;
    uint16_t *MulBeatDataRemain;
    uint32_t TotalSize;

    /* variables used by ISR */
    volatile uint32_t bytes_remaining;
    volatile unsigned int bursts_to_complete;
    volatile uint32_t *readPtr;
    uint32_t finalBurstBuffer[SHIFTNUM];

    void displayInit();
    void CSLow();
    void CSHigh();
    void DCLow();
    void DCHigh();
    void gpioWrite();
    void gpioRead();
    inline void RDHigh();
    inline void RDLow();

    void FlexIO_Init();
    void FlexIO_Config_SnglBeat();
    void FlexIO_Config_MultiBeat();
    void FlexIO_Config_SnglBeat_Read();
    typedef enum { CONFIG_CLEAR = 0,
                   CONFIG_SNGLBEAT,
                   CONFIG_MULTIBEAT,
                   CONFIG_SNGLREAD } Flexio_config_state_t;
    Flexio_config_state_t flex_config = CONFIG_CLEAR;

    

    void microSecondDelay();

    bool isCB = false;
    void _onCompleteCB();

  private:
    int _cs;
    int _rst;

    // The Teensy IO pins used for data and Read and Write 
    uint8_t _data_pins[16], _wr_pin, _rd_pin;  

    uint8_t _flexio_D0, _flexio_WR, _flexio_RD; // which flexio pins do they map to

  protected:
    static void ISR();
    void flexIRQ_Callback();
    static RA8876_t41_p *IRQcallback;

    volatile bool WR_IRQTransferDone = true;
};

#endif
