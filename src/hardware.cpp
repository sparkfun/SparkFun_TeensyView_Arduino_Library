/****************************************************************************** 
hardware.cpp
TeensyView Arduino Library Hardware Interface

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

#include "TeensyView.h"
#include <SPI.h>

/** \brief Set Up SPI Interface

	Sets up the SPI pins, initializes the Arduino's SPI interface.
**/
void TeensyView::spiSetup()
{
	// Initialize the pins:
	// lets see if we have valid pins to use hardware support through spin. 
	_pspin = NULL; 	// assume we will not use the hardware...

#if defined(KINETISK) || defined(KINETISL) 
	if (SPIN.pinIsMOSI(mosiPin) && SPIN.pinIsSCK(sckPin)) {
		_pspin = &SPIN;
	}
	#ifdef SPIN1_OBJECT_CREATED			
	else if (SPIN1.pinIsMOSI(mosiPin) && SPIN1.pinIsSCK(sckPin)) {
		//Serial.println("SPI1 pins");
		_pspin = &SPIN1;
	}
	#endif
	#ifdef SPIN2_OBJECT_CREATED			
	else if (SPIN2.pinIsMOSI(mosiPin) && SPIN2.pinIsSCK(sckPin)) {
		//Serial.println("SPI2 pins");
		_pspin = &SPIN2;
	}
	#endif

	if (_pspin) {

		_pspin->setMOSI(mosiPin);	// set the mosi pin. 
		_pspin->setSCK(sckPin);

		_pspin->begin();		// startup SPI
		//Serial.println("Using SPIN");
#ifdef KINETISK
		_pkinetisk_spi = _pspin->kinetisk_spi(); // get handle on which SPI register set to use
#else
		_pkinetisl_spi = _pspin->kinetisl_spi(); // get handle on which SPI register set to use
#endif		
		// Now see if both cs and dc are chip select pins
#ifdef KINETISK
		if (_pspin->pinIsChipSelect(dcPin, csPin)) {
			//Serial.println("CS and DC both hardware");
			_pcs_data = _pspin->setCS(csPin);
			_pcs_command = _pcs_data | _pspin->setCS(dcPin);
			_csport = 0;	// make sure not to use it...
		} else if (_pspin->pinIsChipSelect(dcPin)) {
			//Serial.println("only DC is hardware");
			_pcs_data = 0;
			_pcs_command = _pcs_data | _pspin->setCS(dcPin);
			pinMode(csPin, OUTPUT);
			_csport    = portOutputRegister(digitalPinToPort(csPin));
			_cspinmask = digitalPinToBitMask(csPin);
			*_csport |= _cspinmask;
		} else 
#endif
		{
			//Serial.println("Not using Hardware CS pins");
			_pcs_data = 0;
			_pcs_command = 0;
			pinMode(csPin, OUTPUT);
			_csport    = portOutputRegister(digitalPinToPort(csPin));
			_cspinmask = digitalPinToBitMask(csPin);
			*_csport |= _cspinmask;
			pinMode(dcPin, OUTPUT);
			_dcport    = portOutputRegister(digitalPinToPort(dcPin));
			_dcpinmask = digitalPinToBitMask(dcPin);
			*_dcport |= _dcpinmask;
			_dcpinAsserted = 0;
		}

	} else {
		Serial.println("Not Valid SPI pins ");
		pinMode(13, OUTPUT);
		while (1) {
			// Blink forever
			digitalWrite(13, !digitalRead(13));
			delay(200);
		}

	}
#else
	//Serial.println("Not Teensy 3.x or LC not using SPIN");
	pinMode(csPin, OUTPUT);	// CS is an OUTPUT
	digitalWrite(csPin, HIGH);	// Start CS High
	
	//Do alt pin assignment
	SPI.setMOSI(mosiPin);
	SPI.setSCK(sckPin);
	
	SPI.begin();
#endif	
}

/** \brief Transfer a byte over SPI

	Use the SPI library to transfer a byte. Only used for data OUTPUT.
	This function does not toggle the CS pin. Do that before and after!
**/
void TeensyView::spiTransfer(byte data)
{
//	SPI.beginTransaction(SPISettings(clockRateSetting, MSBFIRST, SPI_MODE0));
    digitalWrite(csPin, LOW);
    if (_pspin)
    	_pspin->transfer(data);
    else
		SPI.transfer(data);	
    digitalWrite(csPin, HIGH);
//    SPI.endTransaction();
}

