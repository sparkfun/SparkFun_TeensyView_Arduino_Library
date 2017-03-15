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
	pinMode(csPin, OUTPUT);	// CS is an OUTPUT
	digitalWrite(csPin, HIGH);	// Start CS High
	
	//Do alt pin assignment
	SPI.setMOSI(mosiPin);
	SPI.setSCK(sckPin);
	
	SPI.begin();
	
	
}

/** \brief Transfer a byte over SPI

	Use the SPI library to transfer a byte. Only used for data OUTPUT.
	This function does not toggle the CS pin. Do that before and after!
**/
void TeensyView::spiTransfer(byte data)
{
	SPI.beginTransaction(SPISettings(clockRateSetting, MSBFIRST, SPI_MODE0));
    digitalWrite(csPin, LOW);
	SPI.transfer(data);	
    digitalWrite(csPin, HIGH);
    SPI.endTransaction();
}

