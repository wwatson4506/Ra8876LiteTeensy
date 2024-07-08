//**************************************************************//
// Teensy 4.1 8080 Parallel 8/16 bit with 8 bit ASYNC support.
//**************************************************************//
/*
 * Ra8876LiteTeensy.cpp
 * Modified Version of: File Name : RA8876_t41_p.cpp
 *          Author    : RAiO Application Team
 *          Edit Date : 09/13/2017
 *          Version   : v2.0  1.modify bte_DestinationMemoryStartAddr bug
 *                            2.modify ra8876SdramInitial Auto_Refresh
 *                            3.modify ra8876PllInitial
 ****************************************************************
 *                    : New 8080 Parallel version
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
#ifndef _RA8876_T3_P
#define _RA8876_T3_P

#include "Arduino.h"
#include "RA8876Registers.h"
#include "RA8876_common.h"

#include "SPI.h"
// Default to a relatively slow speed for breadboard testing.
// const ru32 SPIspeed = 47000000;
const ru32 SPIspeed = 3000000;

class RA8876_t3 : public RA8876_common {
  public:
    RA8876_t3(const uint8_t CSp = 10, const uint8_t RSTp = 8, const uint8_t mosi_pin = 11, const uint8_t sclk_pin = 13, const uint8_t miso_pin = 12);

    volatile bool activeDMA = false; // Unfortunately must be public so asyncEventResponder() can set it
    bool DMAFinished() {return !activeDMA;}
    
    boolean begin(uint32_t spi_clock = SPIspeed);

    /*access*/
    void lcdRegWrite(ru8 reg, bool finalize = true);
    void lcdDataWrite(ru8 data, bool finalize = true);
    ru8 lcdDataRead(bool finalize = true);
    ru16 lcdDataRead16bpp(bool finalize = true);
    ru8 lcdStatusRead(bool finalize = true);
    void lcdRegDataWrite(ru8 reg, ru8 data, bool finalize = true);
    ru8 lcdRegDataRead(ru8 reg, bool finalize = true);
    void lcdDataWrite16bbp(ru16 data, bool finalize = true);

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
    void putPicture_16bpp(ru16 x, ru16 y, ru16 width, ru16 height);                                   // not recommended: use BTE instead
    void putPicture_16bppData8(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned char *data);   // not recommended: use BTE instead
    void putPicture_16bppData16(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned short *data); // not recommended: use BTE instead

    //. From Onewire utility files
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    void DIRECT_WRITE_LOW(volatile uint32_t *base, uint32_t mask) __attribute__((always_inline)) {
        *(base + 34) = mask;
    }
    void DIRECT_WRITE_HIGH(volatile uint32_t *base, uint32_t mask) __attribute__((always_inline)) {
        *(base + 33) = mask;
    }
#endif

    // SPI Functions - should these be private?
    inline __attribute__((always_inline)) void startSend() {
#ifdef SPI_HAS_TRANSFER_ASYNC
        while (activeDMA) {
        }; // wait forever while DMA is finishing- can't start a new transfer
#endif
        if (!RA8876_BUSY) {
            RA8876_BUSY = true;
            _pspi->beginTransaction(SPISettings(_SPI_CLOCK, MSBFIRST, SPI_MODE0));
        }
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
        DIRECT_WRITE_LOW(_csport, _cspinmask);
#else
        *_csport &= ~_cspinmask;
#endif
    }

    inline __attribute__((always_inline)) void endSend(bool finalize) {
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
        DIRECT_WRITE_HIGH(_csport, _cspinmask);
#else
        *_csport |= _cspinmask;
#endif
        if (finalize) {
            _pspi->endTransaction();
            RA8876_BUSY = false;
        }
    }

    void LCD_CmdWrite(unsigned char cmd);

  private:
    // int _xnscs, _xnreset;
    int _mosi;
    int _miso;
    int _sclk;
    int _cs;
    int _rst;
    int _errorCode;
    SPIClass *_pspi = nullptr;
    //	SPIClass::SPI_Hardware_t *_spi_hardware;

    uint8_t _spi_num;         // Which buss is this spi on?
    uint32_t _SPI_CLOCK;      // #define ILI9341_SPICLOCK 30000000
    uint32_t _SPI_CLOCK_READ; // #define ILI9341_SPICLOCK_READ 2000000

#if defined(KINETISK)
    KINETISK_SPI_t *_pkinetisk_spi;
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    IMXRT_LPSPI_t *_pimxrt_spi;

#elif defined(KINETISL)
    KINETISL_SPI_t *_pkinetisl_spi;
#endif

#ifdef SPI_HAS_TRANSFER_ASYNC
    EventResponder finishedDMAEvent;
#endif
    // add support to allow only one hardware CS (used for dc)
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    uint32_t _cspinmask;
    volatile uint32_t *_csport;
    uint32_t _spi_tcr_current;
#else
    uint8_t _cspinmask;
    volatile uint8_t *_csport;
#endif

  protected:
};

#endif
