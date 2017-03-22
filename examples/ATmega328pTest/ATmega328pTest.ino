/******************************************************************************
  ATmega328pTest.ino
  Example drawing rectangle with Uno

  Marshall Taylor @ SparkFun Electronics, December 6, 2016
  https://github.com/sparkfun/SparkFun_TeensyView_Arduino_Library

  This example displays alternating 50% black frames as fast as possible, reporting
  time between frames.

  Compatible with:
  328p architectures 

  Resources:
  Requires the Teensyduino add on.

  Development environment specifics:
  Arduino IDE 1.6.12 w/ Teensyduino 1.31
  Arduino IDE 1.8.1 w/ Teensyduino 1.35
  TeensyView v1.0

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/
#include <SPI.h>  // Include SPI if you're using SPI
#include <TeensyView.h>  // Include the SFE_MicroOLED library

//////////////////////////////////
// TeensyView Object Declaration //
//////////////////////////////////
//Standard
//NOTE:  For Uno, use easy voltage divider of 2.2k/2.2k to connect 5V pins to the 3.3V pins.
//       All teensy pins are inputs
//       Connect 3V pin of teensy to 3.3V regulated supply
//
//       For uno, PIN_SCK and PIN_MOSI have no effect, they are forced to pin 11 and 13.
//       The other pins can be moved
#define PIN_RESET 3
#define PIN_DC    5
#define PIN_CS    10
#define PIN_SCK   13
#define PIN_MOSI  11

TeensyView oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

void setup()
{
  // These three lines of code are all you need to initialize the
  // OLED and print the splash screen.
  Serial.begin(115200);
  // Before you can start using the OLED, call begin() to init
  // all of the pins and configure the OLED.
  oled.begin();
  // clear(ALL) will clear out the OLED's graphic memory.
  // clear(PAGE) will clear the Arduino's display buffer.
  oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
  // To actually draw anything on the display, you must call the
  // display() function.
  oled.display();
  delay(2000);
  Serial.println("STARTED!!!");
}

float rect1[4] = {10, 10, 40, 30};
float rect1Velo[4] = {0.8, 0.6, 1.2, 0.3};
void loop()
{
  oled.clear(PAGE);
  rect1[0] += rect1Velo[0];
  rect1[1] += rect1Velo[1];
  rect1[2] += rect1Velo[2];
  rect1[3] += rect1Velo[3];
	if(rect1[0] > 127)
	{
		rect1[0] = 127;
		rect1Velo[0] *= -1;
	}
	if(rect1[0] < 0)
	{
		rect1[0] = 0 ;
		rect1Velo[0] *= -1;
	}
	if(rect1[1] > 31)
	{
		rect1[1] = 31;
		rect1Velo[1] *= -1;
	}
	if(rect1[1] < 0)
	{
		rect1[1] = 0 ;
		rect1Velo[1] *= -1;
	}
	if(rect1[2] > 127)
	{
		rect1[2] = 127;
		rect1Velo[2] *= -1;
	}
	if(rect1[2] < 0)
	{
		rect1[2] = 0 ;
		rect1Velo[2] *= -1;
	}
	if(rect1[3] > 31)
	{
		rect1[3] = 31;
		rect1Velo[3] *= -1;
	}
	if(rect1[3] < 0)
	{
		rect1[3] = 0 ;
		rect1Velo[3] *= -1;
	}
	int16_t xOrg = rect1[0];
	if(rect1[2] < xOrg)xOrg = rect1[2];
	int16_t yOrg = rect1[1];
	if(rect1[3] < yOrg)yOrg = rect1[3];
	int16_t xWidth = rect1[0] - rect1[2];
	if(xWidth < 0) xWidth *= -1;
	int16_t yHeight = rect1[1] - rect1[3];
	if(yHeight < 0) yHeight *= -1;
  oled.rect(xOrg, yOrg, xWidth, yHeight);
  oled.display();
  delay(20);



}