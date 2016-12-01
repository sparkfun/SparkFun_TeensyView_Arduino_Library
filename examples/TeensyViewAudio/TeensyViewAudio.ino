/******************************************************************************
<filename>
<Title>
<name @ SparkFun Electronics>
<original creation date>
<github repository address>

<multiline verbose description of file functionality>

Resources:
<additional library requirements>

Development environment specifics:
<arduino/development environment version>
<hardware version>
<etc>

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.
******************************************************************************/
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            audioInput;     //xy=458,218
AudioAnalyzeFFT1024      LeftFFT;          //xy=672,138
AudioAnalyzeFFT1024      RightFFT;      //xy=683,295
AudioOutputI2S           audioOutput;    //xy=686,219
AudioConnection          patchCord1(audioInput, 0, LeftFFT, 0);
AudioConnection          patchCord2(audioInput, 0, audioOutput, 0);
AudioConnection          patchCord3(audioInput, 1, audioOutput, 1);
AudioConnection          patchCord4(audioInput, 1, RightFFT, 0);
AudioControlSGTL5000     audioShield;    //xy=467,310
// GUItool: end automatically generated code


const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;





#include "TeensyView.h"  // Include the SFE_MicroOLED library

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////

////Default SPI connections
//#define PIN_RESET 6  // Connect RST to pin 6
//#define PIN_DC    5  // Connect DC to pin 5
//#define PIN_CS    10 // Connect CS to pin 10
//#define PIN_SCK   13 // Connect SCK to pin 13
//#define PIN_MOSI  11 // Connect MOSI to pin 11

//Alternate SPI connections
#define PIN_RESET 2  // Connect RST to pin 2
#define PIN_DC    21  // Connect DC to pin 21
#define PIN_CS    20 // Connect CS to pin 20
#define PIN_SCK   14 // Connect SCK to pin 14
#define PIN_MOSI  7  // Connect MOSI to pin 7

MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

void setup()
{
	// Set up audio stuff:
	// Audio connections require memory to work.  For more
	// detailed information, see the MemoryAndCpuUsage example
	AudioMemory(20);
	
	// Enable the audio shield and set the output volume.
	audioShield.enable();
	audioShield.inputSelect(myInput);
	audioShield.volume(0.5); //Pass-through volume
	
	// Configure the window algorithm to use
	LeftFFT.windowFunction(AudioWindowHanning1024);
	RightFFT.windowFunction(AudioWindowHanning1024);

	//Initialize the OLED
	oled.begin();
	// clear(ALL) will clear out the OLED's graphic memory.
	// clear(PAGE) will clear the Arduino's display buffer.
	oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
	// To actually draw anything on the display, you must call the
	// display() function. 
	oled.display(); 
	// Give the splash screen some time to shine
	delay(2000);  
}

unsigned long last_time = millis();
uint8_t overlayCounter = 0;
float lastLoopTime = 0;
float lastCPU = 0;
float lastMem = 0;

float leftBands[40] = {
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0
};

float RightBands[40] = {
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0
};

void loop()
{
	float loopTime;
	int i;

	//calc loopTime
	unsigned long this_time = millis();
	if(this_time > last_time)
	{
		loopTime = (this_time - last_time);
	}
	last_time = this_time;

	//Update data every 20 frames for readability
	overlayCounter++;
	if(overlayCounter > 20)
	{
		lastLoopTime = loopTime;
		lastCPU = AudioProcessorUsage();
		lastMem = AudioMemoryUsage();

		overlayCounter = 0;
	}
	
	//Draw a frame
	oled.clear(PAGE);

	//Draw left bands
	for(i = 0; i < 40; i++)
	{
		if(leftBands[i] > 0.5) leftBands[i] = 0.25;
		oled.line(62 - i, 31, 62 - i, 31 - (leftBands[i] * 127));
	}

	//Draw Right bands
	for(i = 0; i < 40; i++)
	{
		if(RightBands[i] > 0.5) RightBands[i] = 0.25;
		oled.line(65 + i, 31, 65 + i, 31 - (RightBands[i] * 127));
	}
	
	//Overlay info
	//  loop time
	oled.setCursor(0,0);
	oled.print("Loop=");
	oled.print((uint8_t)lastLoopTime);
	oled.print("ms");
	//  Teensy Audio info
	oled.setCursor(79,0);
	oled.print("cpu=");
	oled.print(lastCPU, 2);
	oled.setCursor(91,8);
	oled.print("mem=");
	oled.print(lastMem, 0);
	//  L/R letters
	oled.setCursor(15,24);
	oled.print("L");
	oled.setCursor(108, 24);
	oled.print("R");

	if (LeftFFT.available()) {
		// each time new FFT data is available
		for (i=0; i<40; i++) {
			leftBands[i] = LeftFFT.read(i);
		}
	}
	if (RightFFT.available()) {
		// each time new FFT data is available
		for (i=0; i<40; i++) {
			RightBands[i] = RightFFT.read(i);
		}
	}

	oled.display();   

}