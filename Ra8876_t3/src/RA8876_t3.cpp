//**************************************************************//
//**************************************************************//
/*
 * Ra8876LiteTeensy.cpp
 * Modified Version of: File Name : RA8876_t3.cpp
 *			Author    : RAiO Application Team
 *			Edit Date : 09/13/2017
 *			Version   : v2.0  1.modify bte_DestinationMemoryStartAddr bug
 *                 			  2.modify ra8876SdramInitial Auto_Refresh
 *                 			  3.modify ra8876PllInitial
 * 	  	      : For Teensy 3.x and T4
 *                    : By Warren Watson
 *                    : 06/07/2018 - 11/31/2019
 *                    : Copyright (c) 2017-2019 Warren Watson.
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

#include "RA8876_t3.h"
#include "Arduino.h"

#include "SPI.h"
#ifdef SPI_HAS_TRANSFER_ASYNC
#include "EventResponder.h"
#endif

#ifdef SPI_HAS_TRANSFER_ASYNC
//**************************************************************//
// If using DMA, must close transaction and de-assert _CS
// after the data has been sent.
//**************************************************************//
void asyncEventResponder(EventResponderRef event_responder) {
    RA8876_t3 *tft = (RA8876_t3 *)event_responder.getContext();
    tft->activeDMA = false;
    tft->endSend(true);
}
#endif

//**************************************************************//
// RA8876_t3()
//**************************************************************//
// Create RA8876 driver instance
RA8876_t3::RA8876_t3(const uint8_t CSp, const uint8_t RSTp, const uint8_t mosi_pin, const uint8_t sclk_pin, const uint8_t miso_pin) {
    _mosi = mosi_pin;
    _miso = miso_pin;
    _sclk = sclk_pin;
    _cs = CSp;
    _rst = RSTp;
    RA8876_GFX(8);  // Assumes 8 bit 
}

//**************************************************************//
// Ra8876_begin()
//**************************************************************//
#ifndef FLASHMEM
#define FLASHMEM
#endif
FLASHMEM boolean RA8876_t3::begin(uint32_t spi_clock) {
    // initialize the bus for Teensy 3.6/4.0
    //  Figure out which SPI Buss to use.
    _SPI_CLOCK = spi_clock; // #define ILI9341_SPICLOCK 30000000
    if (SPI.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI.pinIsMISO(_miso)) && SPI.pinIsSCK(_sclk)) {
        _pspi = &SPI;
        _spi_num = 0; // Which buss is this spi on?
#ifdef KINETISK
        _pkinetisk_spi = &KINETISK_SPI0; // Could hack our way to grab this from SPI object, but...
//_fifo_full_test = (3 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
        _pimxrt_spi = &IMXRT_LPSPI4_S; // Could hack our way to grab this from SPI object, but...
#else
        _pkinetisl_spi = &KINETISL_SPI0;
#endif

#if defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__) || defined(__MKL26Z64__)
    } else if (SPI1.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI1.pinIsMISO(_miso)) && SPI1.pinIsSCK(_sclk)) {
        _pspi = &SPI1;
        _spi_num = 1; // Which buss is this spi on?
#ifdef KINETISK
        _pkinetisk_spi = &KINETISK_SPI1; // Could hack our way to grab this from SPI object, but...
//_fifo_full_test = (0 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
        _pimxrt_spi = &IMXRT_LPSPI3_S; // Could hack our way to grab this from SPI object, but...
#else
        _pkinetisl_spi = &KINETISL_SPI1;
#endif
#if !defined(__MKL26Z64__)
    } else if (SPI2.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI2.pinIsMISO(_miso)) && SPI2.pinIsSCK(_sclk)) {
        _pspi = &SPI2;
        _spi_num = 2; // Which buss is this spi on?
#ifdef KINETISK
        _pkinetisk_spi = &KINETISK_SPI2; // Could hack our way to grab this from SPI object, but...
//_fifo_full_test = (0 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
        _pimxrt_spi = &IMXRT_LPSPI1_S; // Could hack our way to grab this from SPI object, but...
#endif
#endif
#endif
    } else {
        Serial.println("RA8876_t3: The IO pins on the constructor are not valid SPI pins");

        Serial.printf("    mosi:%d miso:%d SCLK:%d CS:%d\n", _mosi, _miso, _sclk, _cs);
        Serial.flush();
        _errorCode |= (1 << 1); // set
        return false;           // most likely will go bomb
    }
    // Make sure we have all of the proper SPI pins selected.
    _pspi->setMOSI(_mosi);
    _pspi->setSCK(_sclk);
    if (_miso != 0xff)
        _pspi->setMISO(_miso);

    // And startup SPI...
    _pspi->begin();

    // for this round will punt on trying to use CS as hardware CS...
#ifdef KINETISK
    pinMode(_cs, OUTPUT);
    _csport = portOutputRegister(digitalPinToPort(_cs));
    _cspinmask = digitalPinToBitMask(_cs);
    *_csport |= _cspinmask;

#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
    // Serial.println("   T4 setup CS/DC"); Serial.flush();
    _csport = portOutputRegister(_cs);
    _cspinmask = digitalPinToBitMask(_cs);
    pinMode(_cs, OUTPUT);
    DIRECT_WRITE_HIGH(_csport, _cspinmask);
    //	maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(7));

#else
    // TLC
    pinMode(_cs, OUTPUT);
    _csport = portOutputRegister(digitalPinToPort(_cs));
    _cspinmask = digitalPinToBitMask(_cs);
    *_csport |= _cspinmask;
#endif

#ifdef SPI_HAS_TRANSFER_ASYNC
    finishedDMAEvent.setContext(this); // Set the contxt to us
    finishedDMAEvent.attachImmediate(asyncEventResponder);
#endif

    // toggle RST low to reset
    if (_rst < 255) {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(5);
        digitalWrite(_rst, LOW);
        delay(20);
        digitalWrite(_rst, HIGH);
        delay(150);
    }

    if (!checkIcReady())
        return false;
    // read ID code must disable pll, 01h bit7 set 0
    lcdRegDataWrite(0x01, 0x08);
    delay(1);
    if ((lcdRegDataRead(0xff) != 0x76) && (lcdRegDataRead(0xff) != 0x77))
        return false;

    // Initialize RA8876 to default settings
    if (!ra8876Initialize())
        return false;

    // return success
    return true;
}
//**************************************************************//
// Write to a RA8876 register
//**************************************************************//
void RA8876_t3::lcdRegWrite(ru8 reg, bool finalize) {
    ru16 _data = (RA8876_SPI_CMDWRITE16 | reg);

    startSend();
    _pspi->transfer16(_data);
    endSend(finalize);
}

void RA8876_t3::LCD_CmdWrite(unsigned char cmd) {
    startSend();
    _pspi->transfer16(0x00);
    _pspi->transfer(cmd);
    endSend(true);
}

//**************************************************************//
// Write RA8876 Data
//**************************************************************//
void RA8876_t3::lcdDataWrite(ru8 data, bool finalize) {
    ru16 _data = (RA8876_SPI_DATAWRITE16 | data);
    startSend();
    _pspi->transfer16(_data);
    endSend(finalize);
}

//**************************************************************//
// Read RA8876 Data
//**************************************************************//
ru8 RA8876_t3::lcdDataRead(bool finalize) {
    ru16 _data = (RA8876_SPI_DATAREAD16 | 0x00);

    startSend();
    ru8 data = _pspi->transfer16(_data);
    endSend(finalize);
    return data;
}

//**************************************************************//
// Read RA8876 status register
//**************************************************************//
ru8 RA8876_t3::lcdStatusRead(bool finalize) {
    startSend();
    ru8 data = _pspi->transfer16(RA8876_SPI_STATUSREAD16);
    endSend(finalize);
    return data;
}

//**************************************************************//
// Write Data to a RA8876 register
//**************************************************************//
void RA8876_t3::lcdRegDataWrite(ru8 reg, ru8 data, bool finalize) {
    // write the register we wish to write to, then send the data
    // don't need to release _CS between the two transfers
    // ru16 _reg = (RA8876_SPI_CMDWRITE16 | reg);
    // ru16 _data = (RA8876_SPI_DATAWRITE16 | data);
    uint8_t buf[4] = {RA8876_SPI_CMDWRITE, reg, RA8876_SPI_DATAWRITE, data};
    startSend();
    //_pspi->transfer16(_reg);
    //_pspi->transfer16(_data);
    _pspi->transfer(buf, nullptr, 4);
    endSend(finalize);
}

//**************************************************************//
// Read a RA8876 register Data
//**************************************************************//
ru8 RA8876_t3::lcdRegDataRead(ru8 reg, bool finalize) {
    lcdRegWrite(reg, finalize);
    return lcdDataRead();
}

//**************************************************************//
// support SPI interface to write 16bpp data after Regwrite 04h
//**************************************************************//
void RA8876_t3::lcdDataWrite16bbp(ru16 data, bool finalize) {
    startSend();
    _pspi->transfer(RA8876_SPI_DATAWRITE);
    _pspi->transfer16(data);
    endSend(finalize);
}

//**************************************************************//
// Send data from the microcontroller to the RA8876
// Does a Raster OPeration to combine with an image already in memory
// For a simple overwrite operation, use ROP 12
//**************************************************************//
void RA8876_t3::bteMpuWriteWithROPData8(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width,
                                        ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code, const unsigned char *data) {
    bteMpuWriteWithROP(s1_addr, s1_image_width, s1_x, s1_y, des_addr, des_image_width, des_x, des_y, width, height, rop_code);

    startSend();
    _pspi->transfer(RA8876_SPI_DATAWRITE);

#ifdef SPI_HAS_TRANSFER_ASYNC
    activeDMA = true;
    _pspi->transfer(data, NULL, width * height * 2, finishedDMAEvent);
#else
    // If you try _pspi->transfer(data, length) then this tries to write received data into the data buffer
    // but if we were given a PROGMEM (unwriteable) data pointer then _pspi->transfer will lock up totally.
    // So we explicitly tell it we don't care about any return data.
    _pspi->transfer(data, NULL, width * height * 2);
    endSend(true);
#endif
}
//**************************************************************//
// For 16-bit byte-reversed data.
// Note this is 4-5 milliseconds slower than the 8-bit version above
// as the bulk byte-reversing SPI transfer operation is not available
// on all Teensys.
//**************************************************************//
void RA8876_t3::bteMpuWriteWithROPData16(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width,
                                         ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code, const unsigned short *data) {
    ru16 i, j;
    bteMpuWriteWithROP(s1_addr, s1_image_width, s1_x, s1_y, des_addr, des_image_width, des_x, des_y, width, height, rop_code);

    startSend();
    _pspi->transfer(RA8876_SPI_DATAWRITE);

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            _pspi->transfer16(*data);
            data++;
        }
    }

    endSend(true);
}

//==========================================================================================

//**************************************************************//
/* Write 16bpp(RGB565) picture data for user operation          */
/* Not recommended for future use - use BTE instead             */
//**************************************************************//
void RA8876_t3::putPicture_16bpp(ru16 x, ru16 y, ru16 width, ru16 height) {
    graphicMode(true);
    activeWindowXY(x, y);
    activeWindowWH(width, height);
    setPixelCursor(x, y);
    ramAccessPrepare();
    // Now your program has to send the image data pixels
}

//*******************************************************************//
/* write 16bpp(RGB565) picture data in byte format from data pointer */
/* Not recommended for future use - use BTE instead                  */
//*******************************************************************//
void RA8876_t3::putPicture_16bppData8(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned char *data) {
    ru16 i, j;
    graphicMode(true);
    activeWindowXY(x, y);
    activeWindowWH(width, height);
    setPixelCursor(x, y);
    ramAccessPrepare();
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            // checkWriteFifoNotFull();  //if high speed mcu and without Xnwait check
            lcdDataWrite(*data);
            data++;
            // checkWriteFifoNotFull();  //if high speed mcu and without Xnwait check
            lcdDataWrite(*data);
            data++;
        }
    }
    checkWriteFifoEmpty(); // if high speed mcu and without Xnwait check
    activeWindowXY(0, 0);
    activeWindowWH(_width, _height);
}

//****************************************************************//
/* Write 16bpp(RGB565) picture data word format from data pointer */
/* Not recommended for future use - use BTE instead               */
//****************************************************************//
void RA8876_t3::putPicture_16bppData16(ru16 x, ru16 y, ru16 width, ru16 height, const unsigned short *data) {
    ru16 i, j;
    putPicture_16bpp(x, y, width, height);
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            // checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
            lcdDataWrite16bbp(*data);
            data++;
            // checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
        }
    }
    checkWriteFifoEmpty(); // if high speed mcu and without Xnwait check
    activeWindowXY(0, 0);
    activeWindowWH(_width, _height);
}

//**************************************************************//
// write data after setting, using lcdDataWrite() or lcdDataWrite16bbp()
//**************************************************************//
void RA8876_t3::bteMpuWriteWithROP(ru32 s1_addr, ru16 s1_image_width, ru16 s1_x, ru16 s1_y, ru32 des_addr, ru16 des_image_width, ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru8 rop_code) {
    check2dBusy();
    graphicMode(true);
    bte_Source1_MemoryStartAddr(s1_addr);
    bte_Source1_ImageWidth(s1_image_width);
    bte_Source1_WindowStartXY(s1_x, s1_y);
    bte_DestinationMemoryStartAddr(des_addr);
    bte_DestinationImageWidth(des_image_width);
    bte_DestinationWindowStartXY(des_x, des_y);
    bte_WindowSize(width, height);
    lcdRegDataWrite(RA8876_BTE_CTRL1, rop_code << 4 | RA8876_BTE_MPU_WRITE_WITH_ROP);                                                             // 91h
    lcdRegDataWrite(RA8876_BTE_COLR, RA8876_S0_COLOR_DEPTH_16BPP << 5 | RA8876_S1_COLOR_DEPTH_16BPP << 2 | RA8876_DESTINATION_COLOR_DEPTH_16BPP); // 92h
    lcdRegDataWrite(RA8876_BTE_CTRL0, RA8876_BTE_ENABLE << 4);                                                                                    // 90h
    ramAccessPrepare();
}

//**************************************************************//
// Send data from the microcontroller to the RA8876
// Does a chromakey (transparent color) to combine with the image already in memory
//**************************************************************//
void RA8876_t3::bteMpuWriteWithChromaKeyData8(ru32 des_addr, ru16 des_image_width, ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru16 chromakey_color, const unsigned char *data) {
    bteMpuWriteWithChromaKey(des_addr, des_image_width, des_x, des_y, width, height, chromakey_color);

    startSend();
    _pspi->transfer(RA8876_SPI_DATAWRITE);

#ifdef SPI_HAS_TRANSFER_ASYNC
    activeDMA = true;
    _pspi->transfer(data, NULL, width * height * 2, finishedDMAEvent);
#else
    _pspi->transfer(data, NULL, width * height * 2);
    endSend(true);
#endif
}
//**************************************************************//
// Chromakey for 16-bit byte-reversed data. (Slower than 8-bit.)
//**************************************************************//
void RA8876_t3::bteMpuWriteWithChromaKeyData16(ru32 des_addr, ru16 des_image_width, ru16 des_x, ru16 des_y, ru16 width, ru16 height, ru16 chromakey_color, const unsigned short *data) {
    ru16 i, j;
    bteMpuWriteWithChromaKey(des_addr, des_image_width, des_x, des_y, width, height, chromakey_color);

    startSend();
    _pspi->transfer(RA8876_SPI_DATAWRITE);
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            _pspi->transfer16(*data);
            data++;
        }
    }
    endSend(true);
}
