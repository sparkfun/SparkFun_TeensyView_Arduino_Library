/****************************************************************************** 
TeensyView.h
Header file for the TeensyView Library

Marshall Taylor @ SparkFun Electronics, December 6, 2016
https://github.com/sparkfun/SparkFun_TeensyView_Arduino_Library

This has been modified from the SFE_MicroOLED library, Original contributors:

Jim Lindblom @ SparkFun Electronics, October 26, 2014
https://github.com/sparkfun/Micro_OLED_Breakout/tree/master/Firmware/Arduino/libraries/SFE_MicroOLED

Emil Varughese @ Edwin Robotics Pvt. Ltd. July 27, 2015
https://github.com/emil01/SparkFun_Micro_OLED_Arduino_Library/

GeekAmmo, (https://github.com/geekammo/MicroView-Arduino-Library)

Released under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*****This file defines the hardware interface(s) for the TeensyView.*****

Development environment specifics:
Arduino IDE 1.6.12 w/ Teensyduino 1.31
Arduino IDE 1.8.1 w/ Teensyduino 1.35
TeensyView v1.0

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef TEENSYVIEW_H
#define TEENSYVIEW_H

#include <stdio.h>

#ifdef __cplusplus
#include "Arduino.h"
#include <SPI.h>
#include <SPIN.h>
#include <DMAChannel.h>

#endif

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 0
#define WHITE 1

#define LCDWIDTH			128
#define LCDHEIGHT			32
#define LCDMEMORYSIZE		( LCDWIDTH * LCDHEIGHT / 8 )

#define FONTHEADERSIZE		6
// The font header size is the first bytes of the static const unsigned char space that are not data.
// They are defined in the font files.
//   first row defines - FONTWIDTH, FONTHEIGHT, ASCII START CHAR, TOTAL CHARACTERS, FONT MAP WIDTH HIGH, FONT MAP WIDTH LOW (2,56 meaning 256)

#define NORM				0
#define XOR					1

#define PAGE				0
#define ALL					1

#define SETCONTRAST 		0x81
#define DISPLAYALLONRESUME 	0xA4
#define DISPLAYALLON 		0xA5
#define NORMALDISPLAY 		0xA6
#define INVERTDISPLAY 		0xA7
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define SETDISPLAYOFFSET 	0xD3
#define SETCOMPINS 			0xDA
#define SETVCOMDESELECT		0xDB
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETPRECHARGE 		0xD9
#define SETMULTIPLEX 		0xA8
#define SETLOWCOLUMN 		0x00
#define SETHIGHCOLUMN 		0x10
#define SETSTARTLINE 		0x40
#define MEMORYMODE 			0x20
#define COMSCANINC 			0xC0
#define COMSCANDEC 			0xC8
#define SEGREMAP 			0xA0
#define CHARGEPUMP 			0x8D
#define EXTERNALVCC 		0x01
#define SWITCHCAPVCC 		0x02

// Scroll
#define ACTIVATESCROLL 					0x2F
#define DEACTIVATESCROLL 				0x2E
#define SETVERTICALSCROLLAREA 			0xA3
#define RIGHTHORIZONTALSCROLL 			0x26
#define LEFT_HORIZONTALSCROLL 			0x27
#define VERTICALRIGHTHORIZONTALSCROLL	0x29
#define VERTICALLEFTHORIZONTALSCROLL	0x2A

typedef enum CMD {
	CMD_CLEAR,			//0
	CMD_INVERT,			//1
	CMD_CONTRAST,		//2
	CMD_DISPLAY,		//3
	CMD_SETCURSOR,		//4
	CMD_PIXEL,			//5
	CMD_LINE,			//6
	CMD_LINEH,			//7
	CMD_LINEV,			//8
	CMD_RECT,			//9
	CMD_RECTFILL,		//10
	CMD_CIRCLE,			//11
	CMD_CIRCLEFILL,		//12
	CMD_DRAWCHAR,		//13
	CMD_DRAWBITMAP,		//14
	CMD_GETLCDWIDTH,	//15
	CMD_GETLCDHEIGHT,	//16
	CMD_SETCOLOR,		//17
	CMD_SETDRAWMODE		//18
} commCommand_t;

class TeensyView : public Print{
public:
	// Constructor(s)
	TeensyView(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi);
	
	void begin(void);
	void setClockRate( uint32_t );
	virtual size_t write(uint8_t);

	// RAW LCD functions
	void command(uint8_t c, boolean last=1);
	void data(uint8_t c, boolean last=0);
	void setColumnAddress(uint8_t add);
	void setPageAddress(uint8_t add);
	
	// LCD Draw functions
	void clear(uint8_t mode);
	void clear(uint8_t mode, uint8_t c);
	void invert(boolean inv);
	void contrast(uint8_t contrast);
	void display(void);
	void display(uint8_t);
	void setCursor(uint8_t x, uint8_t y);
	void pixel(uint8_t x, uint8_t y);
	void pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode);
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode);
	void lineH(uint8_t x, uint8_t y, uint8_t width);
	void lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode);
	void lineV(uint8_t x, uint8_t y, uint8_t height);
	void lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode);
	void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void circle(uint8_t x, uint8_t y, uint8_t radius);
	void circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color, uint8_t mode);
	void circleFill(uint8_t x0, uint8_t y0, uint8_t radius);
	void circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode);
	void drawChar(uint8_t x, uint8_t y, uint8_t c);
	void drawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode);
	void drawBitmap(uint8_t * bitArray);
	uint8_t getLCDWidth(void);
	uint8_t getLCDHeight(void);
	void setColor(uint8_t color);
	void setDrawMode(uint8_t mode);
	uint8_t *getScreenBuffer(void);

	// Font functions
	uint8_t getFontWidth(void);
	uint8_t getFontHeight(void);
	uint8_t getTotalFonts(void);
	uint8_t getFontType(void);
	uint8_t setFontType(uint8_t type);
	uint8_t getFontStartChar(void);
	uint8_t getFontTotalChar(void);

	// LCD Rotate Scroll functions	
	void scrollRight(uint8_t start, uint8_t stop);
	void scrollLeft(uint8_t start, uint8_t stop);
	void scrollVertRight(uint8_t start, uint8_t stop);
	void scrollVertLeft(uint8_t start, uint8_t stop);
	void scrollStop(void);
	void flipVertical(boolean flip);
	void flipHorizontal(boolean flip);
	
private:
	uint8_t csPin, dcPin, rstPin, sckPin, mosiPin;
//	volatile uint8_t *wrport, *wrreg, *rdport, *rdreg;
//	uint8_t wrpinmask, rdpinmask;
//	volatile uint8_t *ssport, *dcport, *ssreg, *dcreg;	// use volatile because these are fixed location port address
//	uint8_t mosipinmask, sckpinmask, sspinmask, dcpinmask;
	uint8_t foreColor,drawMode,fontWidth, fontHeight, fontType, fontStartChar, fontTotalChar, cursorX, cursorY;
	uint16_t fontMapWidth;
	static const unsigned char *fontsPointer[];
	uint32_t clockRateSetting;
	
	// Communication (Defined in hardware.cpp)
	void spiTransfer(byte data);
	void spiSetup();

	// Stuff added to switch to using SPIN
	uint8_t _pcs_data, _pcs_command;
    volatile uint8_t *_csport, *_dcport;
    uint8_t _cspinmask, _dcpinmask;



 	SPINClass *_pspin;
#ifdef KINETISK	
 	KINETISK_SPI_t *_pkinetisk_spi;
#endif
#ifdef KINETISL
 	KINETISL_SPI_t *_pkinetisl_spi;
#endif	
 	// Inline helper functions
	void beginSPITransaction() __attribute__((always_inline)) {
		if (_pspin) {
			_pspin->beginTransaction(SPISettings(clockRateSetting, MSBFIRST, SPI_MODE0));
		} else {
			SPI.beginTransaction(SPISettings(clockRateSetting, MSBFIRST, SPI_MODE0));
		}
		if (_csport)
			*_csport  &= ~_cspinmask;
	}
	void endSPITransaction() __attribute__((always_inline)) {
		if (_csport)
			*_csport |= _cspinmask;
		if (_pspin) {
			_pspin->endTransaction();
		} else {
			SPI.endTransaction();
		}
	}


#ifdef KINETISK	
	// Always use on TLC, only use on T3.x if DC pin is not on hardware CS pin
	uint8_t _dcpinAsserted;
	void setCommandMode() __attribute__((always_inline)) {
		if (!_dcpinAsserted) {
			*_dcport  &= ~_dcpinmask;
			_dcpinAsserted = 1;
		}
	}

	void setDataMode() __attribute__((always_inline)) {
		if (_dcpinAsserted) {
			*_dcport  |= _dcpinmask;
			_dcpinAsserted = 0;
		}
	}
	void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
		if (!_pcs_data) setCommandMode();
		_pkinetisk_spi->PUSHR = c | (_pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		_pspin->waitFifoNotFull();
	}
	void writedata8_cont(uint8_t c) __attribute__((always_inline)) {
		if (!_pcs_data) setDataMode();
		_pkinetisk_spi->PUSHR = c | (_pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		_pspin->waitFifoNotFull();
	}
	void writedata16_cont(uint16_t d) __attribute__((always_inline)) {
		if (!_pcs_data) setDataMode();
		_pkinetisk_spi->PUSHR = d | (_pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
		_pspin->waitFifoNotFull();
	}
	void writecommand_last(uint8_t c) __attribute__((always_inline)) {
		if (!_pcs_data) setCommandMode();
		uint32_t mcr = _pkinetisk_spi->MCR;
		_pkinetisk_spi->PUSHR = c | (_pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		_pspin->waitTransmitComplete(mcr);
	}
	void writedata8_last(uint8_t c) __attribute__((always_inline)) {
		if (!_pcs_data) setDataMode();
		uint32_t mcr = _pkinetisk_spi->MCR;
		_pkinetisk_spi->PUSHR = c | (_pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		_pspin->waitTransmitComplete(mcr);
	}
	void writedata16_last(uint16_t d) __attribute__((always_inline)) {
		if (!_pcs_data) setDataMode();
		uint32_t mcr = _pkinetisk_spi->MCR;
		_pkinetisk_spi->PUSHR = d | (_pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
		_pspin->waitTransmitComplete(mcr);
	}
#endif
#ifdef KINETISL
	uint8_t _data_sent_not_completed;
	void waitTransmitComplete() __attribute__((always_inline)) {
		if (_data_sent_not_completed) {
			uint8_t timeout_count = 0xff; // hopefully enough 
			while (!(_pkinetisl_spi->S & SPI_S_SPRF) && timeout_count--) ; // wait 
			uint8_t d __attribute__((unused));
			d = _pkinetisl_spi->DL;
			_data_sent_not_completed = false; // We hopefully received our data...
		}
	}

	// Always use on TLC
	uint8_t _dcpinAsserted;
	void setCommandMode() __attribute__((always_inline)) {
		if (!_dcpinAsserted) {
			waitTransmitComplete();
			*_dcport  &= ~_dcpinmask;
			_dcpinAsserted = 1;
		}
	}

	void setDataMode() __attribute__((always_inline)) {
		if (_dcpinAsserted) {
			waitTransmitComplete();
			*_dcport  |= _dcpinmask;
			_dcpinAsserted = 0;
		}
	}

	void outputToSPI(uint8_t c)  __attribute__((always_inline)) {
		while (!(_pkinetisl_spi->S & SPI_S_SPTEF)) ; // wait if output buffer busy.
		// Clear out buffer if there is something there...
		if  ((_pkinetisl_spi->S & SPI_S_SPRF)) {
			uint8_t d __attribute__((unused));
			d = _pkinetisl_spi->DL;
		} 
		_pkinetisl_spi->DL = c; // output byte
		_data_sent_not_completed = 1; // let system know we sent something	
	}

	void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
		setCommandMode();
		outputToSPI(c);
	}
	void writedata8_cont(uint8_t c) __attribute__((always_inline)) {
		setDataMode();
		outputToSPI(c);
	}

	void writecommand_last(uint8_t c) __attribute__((always_inline)) {
		setCommandMode();
		outputToSPI(c);
		waitTransmitComplete();
	}
	void writedata8_last(uint8_t c) __attribute__((always_inline)) {
		setDataMode();
		outputToSPI(c);
		waitTransmitComplete();
	}
#endif

};
#endif
