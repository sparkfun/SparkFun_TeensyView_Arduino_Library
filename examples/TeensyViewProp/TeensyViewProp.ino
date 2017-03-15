/******************************************************************************
  TeensyViewProp.ino
  Example using the TeensyView with the Teensy Prop Shield.

  Marshall Taylor @ SparkFun Electronics, December 6, 2016
  https://github.com/sparkfun/SparkFun_TeensyView_Arduino_Library

  This enables all resources on the Teensy Prop Shield and operates them with
  the TeensyView.

  Accelerometer data is used to drive Teensy Audio system (bends pitch)
  Flash is programmed with LED data at boot (note: comment out flash erasor if necessary)
  LED continuously reads flash file for color information (Driven raw by SPI)
  TeensyView continuously updated with pitch, roll, and heading information.

  Compatible with:
  Teensy 3.1 + Prop Shield
  Teensy 3.2 + Prop Shield
  Teensy 3.5 + Prop Shield
  Teensy 3.6 + Prop Shield

  Resources:
  Requires the Teensy Audio library
  NXPMotionSense Library
  EEPROM Library
  SerialFlash Library


  Development environment specifics:
  Arduino IDE 1.6.12
  TeensyView v1.0

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/


//********************IMU test*******************//
#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
NXPMotionSense imu;
NXPSensorFusion filter;
//********************IMU test*******************//

//********************Audio test*******************//
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine4;          //xy=321,367
AudioSynthWaveformSine   sine3;          //xy=323,314
AudioSynthWaveformSine   sine2;          //xy=324,264
AudioSynthWaveformSine   sine1;          //xy=325,213
AudioMixer4              mixer1;         //xy=506,303
AudioSynthWaveformDc     dc1;            //xy=517,382
AudioFilterStateVariable filter1;        //xy=695,311
AudioFilterStateVariable filter2;        //xy=851,342
AudioOutputAnalog        dac1;           //xy=1028,366
AudioConnection          patchCord1(sine4, 0, mixer1, 3);
AudioConnection          patchCord2(sine3, 0, mixer1, 2);
AudioConnection          patchCord3(sine2, 0, mixer1, 1);
AudioConnection          patchCord4(sine1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, filter1, 0);
AudioConnection          patchCord6(dc1, 0, filter1, 1);
AudioConnection          patchCord7(dc1, 0, filter2, 1);
AudioConnection          patchCord8(filter1, 0, filter2, 0);
AudioConnection          patchCord9(filter2, 0, dac1, 0);
// GUItool: end automatically generated code
//********************Audio test*******************//

//********************Flash Test*******************//
//#include <SerialFlash.h>
//#include <SPI.h>
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
SerialFlashFile file; //Working file
uint16_t fileIndex = 0;
//********************Flash Test*******************//

//********************TeensyView*******************//
#include <TeensyView.h>  // Include the SFE_TeensyView library

///////////////////////////////////
// TeensyView Object Declaration //
///////////////////////////////////
//Standard
#define PIN_RESET 15
//#define PIN_DC    5
//#define PIN_CS    10
#define PIN_SCK   13
#define PIN_MOSI  11

//Alternate (Audio)
//#define PIN_RESET 2
#define PIN_DC    21
#define PIN_CS    20
//#define PIN_SCK   14
//#define PIN_MOSI  7

TeensyView oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);
//********************TeensyView*******************//

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
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("Heading");
  oled.setCursor(50, 0);
  oled.print("Pitch");
  oled.setCursor(90, 0);
  oled.print("Roll");
  oled.line(0, 9, 127, 9);
  oled.display();

  //********************Audio test*******************//
  AudioMemory(40);
  mixer1.gain(0, 0.1);
  mixer1.gain(1, 0.1);
  mixer1.gain(2, 0.1);
  mixer1.gain(3, 0.1);
  filter1.frequency(500);
  filter1.octaveControl(2);
  filter1.resonance(2);
  filter2.frequency(500);
  filter1.octaveControl(2);
  filter2.resonance(2);
  dc1.amplitude(0);
  setSines(110);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH); // turn on the amplifier
  delay(10);
  //********************Audio test*******************//

  Serial.begin(9600);

  //********************IMU test*******************//
  imu.begin();
  filter.begin(100);
  //********************IMU test*******************//

  //********************RAW APA102*******************//
  pinMode(7, OUTPUT); //Configure chip select pin
  //********************RAW APA102*******************//

  //********************Flash Test*******************//
  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println("Unable to access SPI Flash chip");
  }
  uint8_t id[5];
  SerialFlash.readID(id);
  Serial.println("ID word:");
  Serial.print("0x");
  Serial.println(id[0], HEX);
  Serial.print("0x");
  Serial.println(id[1], HEX);
  Serial.print("0x");
  Serial.println(id[2], HEX);
  Serial.print("0x");
  Serial.println(id[3], HEX);
  //clearFlash();  //Use this to erase all... this holds the program in a while loop when done.
  prepareTestFile();  //This creates a test file IF it doesn't exist
  listTestFile();
  //********************Flash Test*******************//

}
//********************RAW APA102*******************//
void setLED( uint8_t r, uint8_t g, uint8_t b )
{
  uint8_t brightness = 0b00001000; //Quarter bright
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(7, HIGH);  // enable access to LEDs
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0b11100000 | brightness);
  SPI.transfer(b);
  SPI.transfer(g);
  SPI.transfer(r);
  digitalWrite(7, LOW);  // enable access to LEDs
  SPI.endTransaction();
  //This sends data to the LED
}
//********************RAW APA102*******************//

//********************Flash Test*******************//
//Test data for mem R/W (also LED display data, as r1, g1, b1, r2, g2, b2...)
uint8_t const colorwheel[128 * 3] = {
  64, 22, 10, 63, 21, 11, 63, 19, 12, 63, 18, 14, 63, 16, 15, 63, 15, 16, 62, 14, 18, 62, 12, 19,
  61, 11, 21, 60, 10, 22, 60, 9, 24, 59, 8, 25, 58, 7, 27, 57, 6, 28, 56, 5, 30, 55, 4, 32,
  54, 3, 33, 53, 3, 35, 52, 2, 36, 51, 1, 38, 49, 1, 39, 48, 0, 41, 47, 0, 42, 45, 0, 44,
  44, 0, 45, 42, 0, 47, 41, 0, 48, 39, 0, 49, 38, 0, 51, 36, 0, 52, 35, 0, 53, 33, 0, 54,
  32, 1, 55, 30, 1, 56, 28, 2, 57, 27, 3, 58, 25, 3, 59, 24, 4, 60, 22, 5, 60, 21, 6, 61,
  19, 7, 62, 18, 8, 62, 16, 9, 63, 15, 10, 63, 14, 11, 63, 12, 12, 63, 11, 14, 63, 10, 15, 64,
  9, 16, 63, 8, 18, 63, 7, 19, 63, 6, 21, 63, 5, 22, 63, 4, 24, 62, 3, 25, 62, 3, 27, 61,
  2, 28, 60, 1, 30, 60, 1, 32, 59, 0, 33, 58, 0, 35, 57, 0, 36, 56, 0, 38, 55, 0, 39, 54,
  0, 41, 53, 0, 42, 52, 0, 44, 51, 0, 45, 49, 0, 47, 48, 0, 48, 47, 1, 49, 45, 1, 51, 44,
  2, 52, 42, 3, 53, 41, 3, 54, 39, 4, 55, 38, 5, 56, 36, 6, 57, 35, 7, 58, 33, 8, 59, 32,
  9, 60, 30, 10, 60, 28, 11, 61, 27, 12, 62, 25, 14, 62, 24, 15, 63, 22, 16, 63, 21, 18, 63, 19,
  19, 63, 18, 21, 63, 16, 22, 64, 15, 24, 63, 14, 25, 63, 12, 27, 63, 11, 28, 63, 10, 30, 63, 9,
  32, 62, 8, 33, 62, 7, 35, 61, 6, 36, 60, 5, 38, 60, 4, 39, 59, 3, 41, 58, 3, 42, 57, 2,
  44, 56, 1, 45, 55, 1, 47, 54, 0, 48, 53, 0, 49, 52, 0, 51, 51, 0, 52, 49, 0, 53, 48, 0,
  54, 47, 0, 55, 45, 0, 56, 44, 0, 57, 42, 0, 58, 41, 0, 59, 39, 1, 60, 38, 1, 60, 36, 2,
  61, 35, 3, 62, 33, 3, 62, 32, 4, 63, 30, 5, 63, 28, 6, 63, 27, 7, 63, 25, 8, 63, 24, 9
};
void clearFlash( void )
{
  Serial.print("Erasing flash");
  SerialFlash.eraseAll();
  while (SerialFlash.ready() == false)
  {
    // wait, 30 seconds to 2 minutes for most chips
    Serial.print(".");
    delay(100);
  }
  Serial.println("Done!");
  Serial.println("Program held.  Now comment out clearFlash(); and recompile");
  while (1);
}

void prepareTestFile( void )
{
  //Check if file exists, if not, create it.
  if (SerialFlash.exists("testfile.dat") == 0)
  {
    //File doesn't exist
    if (SerialFlash.create("testfile.dat", 128 * 3) == true)
    {
      Serial.println("Created testfile.dat");
    }
    else
    {
      Serial.println("File creation failed!!!");
    }
  }
  if (SerialFlash.exists("testfile.dat"))
  {
    Serial.println("File Exists, trying to open...");
    file = SerialFlash.open("testfile.dat");
    if (file)
    { // true if the file exists
      Serial.println("testfile.dat opened");
      uint8_t buffer[3];
      for (int i = 0; i < (128 * 3); i = i + 3)
      {
        buffer[0] = colorwheel[i];
        buffer[1] = colorwheel[i + 1];
        buffer[2] = colorwheel[i + 2];
        file.seek(i);
        file.write(buffer, 3);
        Serial.print(".");
      }
      file.close();
      Serial.println("Test data generation complete");
    }
    else
    {
      Serial.println("testfile.dat not opened!!!");
    }
  }
}

void listTestFile( void )
{
  file = SerialFlash.open("testfile.dat");
  if (file)
  { // true if the file exists
    Serial.println("testfile.dat opened");
    uint8_t buffer[3];
    fileIndex = 0;
    for (int i = 0; i < 128; i++)
    {
      file.seek(fileIndex);
      fileIndex = fileIndex + 3;
      file.read(buffer, 3);
      Serial.println(buffer[0], HEX);
      Serial.println(buffer[1], HEX);
      Serial.println(buffer[2], HEX);
    }
    file.close();
  }
  else
  {
    Serial.println("testfile.dat not opened!!!");
  }
}
//********************Flash Test*******************//

//********************Audio test*******************//
void setSines(float root)
{
  sine1.frequency(root);
  sine1.amplitude(0.25);
  sine2.frequency(root * 2);
  sine2.amplitude(0.25);
  sine3.frequency(root * 3);
  sine3.amplitude(0.25);
  sine4.frequency(root * 4);
  sine4.amplitude(0.25);
}
//********************Audio test*******************//

void loop()
{
  //********************IMU test*******************//
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;
  if (imu.available())
  {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    //********************Audio test*******************//
    dc1.amplitude(pitch / 20);
    setSines(55 + (roll * 10));
    //********************Audio test*******************//

    //********************TeensyView*******************//
    oled.setCursor(0, 13);
    oled.print("                     ");
    oled.setCursor(0, 13);
    oled.print(heading);
    oled.setCursor(43, 13);
    oled.print(pitch);
    oled.setCursor(83, 13);
    oled.print(roll);
    oled.display();
    //********************TeensyView*******************//
  }
  //********************IMU test*******************//

  //********************Flash Test*******************//
  if (fileIndex >= (128 * 3)) fileIndex = 0;
  fileIndex = fileIndex + 3;
  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println("Unable to access SPI Flash chip");
  }
  file = SerialFlash.open("testfile.dat");
  if (file)
  { // true if the file exists
    uint8_t buffer[3];
    file.seek(fileIndex);
    file.read(buffer, 3);
    file.close();
    //Serial.println(buffer[0], HEX); //Show red channel to serial console
    //********************RAW APA102*******************//
    setLED( buffer[0], buffer[1], buffer[2] );
    //********************RAW APA102*******************//
  }
  //********************Flash Test*******************//
}