#include <Adafruit_CircuitPlayground.h>

// POV project
// Basic POV Sketch - Prints static CapShield
// 
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// 
// Hall effect sensor is connected to digital pin 8

#include <Adafruit_DotStar.h>
#define NUMPIXELS 60 // Number of DotStar LEDs on Strip
#define DATAPIN 2 // DotStar DataOutput Pin
#define CLOCKPIN 3 // DotStar ClockOutput Pin
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG); //change BRG to correct dotstar config
 
#define ROTATEINPUT 10  //  Hall effect sensor is connected to this pin
unsigned long povDisplayColumnWidthCount = 0;         // The arctime of a column 
const unsigned int povDisplayColumnCount = 65;      // The number of columns in the display

 
volatile unsigned int povDisplayColumn = 0;         // The next pov display column
unsigned long time1;
unsigned long time2;
unsigned long timeA;
unsigned long timeB;
unsigned long timeC;

 
// Data to be displayed on the pov display
// initialised with static colours
// 

const uint32_t r = 0xFF0000;
const uint32_t w = 0xFFFFFF;
const uint32_t b = 0x0000FF;

uint32_t povDisplayData[povDisplayColumnCount][NUMPIXELS] ={
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , w,w,w,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,w,w,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,w,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,b,b,b,b,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,b,b,b,b,b , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,b,b,b,b,b , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,b,b,b,b,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,b,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,b,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,b,w,w,w,w,w,w,w , w,w,w,w,w,w},
    {r,r,r,r,r,w,w,w,w,w,r,r,r,r,r , b,w,w,w,w,w,w,w,w , w,w,w,w,w,w}
};
 
                                   
void setup()
{
  noInterrupts();         // Turn off interrupts
 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

 //  external configuration
  attachInterrupt(digitalPinToInterrupt(ROTATEINPUT), revolutionDetected, RISING);    //   Hall effect sensor connected to this pin

  interrupts();       // enable interrupts
   
}
 
 
void loop() {         // Empty main program loop
  timeA = micros();
  if (povDisplayColumn < povDisplayColumnCount){
    for (int i = 0; i < NUMPIXELS; i++){
      strip.setPixelColor(
         i,                                       // Pixel Number
         povDisplayData[povDisplayColumn][i]         // Colour as uint32_t 0xFF00FF
      );
    }
    strip.show();                 // Output data to the leds
  } else { 
    strip.clear();                // Turn off the leds  
  }
  povDisplayColumn++;             // increment the display column
  timeB = micros();
  timeC = timeB - timeA;
  delayMicroseconds( povDisplayColumnWidthCount - timeC ); // delay the next column draw by the arctime minus the draw time.
}
 
 
void revolutionDetected(){

  time2 = time1;                                     // Move time1 count to time2
  time1 = micros();                                  // Get time in uSeconds
  povDisplayColumnWidthCount = ( time1 - time2 ) / povDisplayColumnCount;    // Calculate the duration for a column in uSeconds
  //povDisplayColumn=0;                              // Reset display to the first column keeps video vertical prevents drift

}
