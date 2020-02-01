#include <Adafruit_CircuitPlayground.h>

// POV project
// Basic POV Sketch - Prints static text
// techydiy.org
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// 
// Hall effect sensor is connected to ICP1 digital pin 8
// LEDS connected to port D
#include <Adafruit_DotStar.h>
#define NUMPIXELS 60 // Number of DotStar LEDs on Strip
#define DATAPIN 2 // DotStar DataOutput Pin
#define CLOCKPIN 3 // DotStar ClockOutput Pin
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG); //change BRG to correct dotstar config
 
const byte inputCapturePin = 10;  //  Hall effect sensor is connected to this pin
volatile unsigned int povDisplayColumnWidthCount = 0;         // The count width of a column 
const unsigned int povDisplayColumnCount = 65;      // The number of columns in the display

 
volatile byte timer1CountOverflows = 0;     // Timer 1 counter overflows
volatile unsigned int povDisplayColumn = 0;         // The next pov display column

 
// Data to be displayed on the pov display
// initialised with static colours
// 
// 
const byte povDisplayData[povDisplayColumnCount][NUMPIXELS][3] ={
    {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
     {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
      {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
       {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
        {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
         {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
          {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
          {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
         {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
        {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
       {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
      {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}},
     {{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00},{0xFF, 0x00, 0x00} , {0x00, 0x00, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF} , {0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF},{0xFF, 0xFF, 0xFF}}
};
 
                                   
void setup()
{
  noInterrupts();         // Turn off interrupts
 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

 //  external configuration
  attachInterrupt(digitalPinToInterrupt(inputCapturePin), revolutionDetected, RISING);    //   Hall effect sensor connected to this pin

  interrupts();       // enable interrupts
   
}
 
 
void loop() {         // Empty main program loop
  timeA = micros()
  if (povDisplayColumn < povDisplayColumnCount){
    for (int i = 0; i < NUMPIXELS; i++){
      strip.setPixelColor(
         i,                                       // Pixel Number
         povDisplayData[povDisplayColumn][i][0],  // Pixel LED Red
         povDisplayData[povDisplayColumn][i][1],  // Pixel LED Green
         povDisplayData[povDisplayColumn][i][2]   // Pixel LED Blue
      );
    }
    strip.show();                 // Output data to the leds
  } else { 
    strip.clear();                // Turn off the leds  
  }
  povDisplayColumn++;             // increment the display column
  timeB = micros()
  timeC = timeB - timeA
  delayMicroseconds( povDisplayColumnWidthCount - timeC ) // delay the next column draw by the arctime minus the draw time.
}
 
 
void revolutionDetected(){

  time2 = time1;                                // Move time1 count to time2
  time1 = micros();                             // Get time in uSeconds
  povDisplayColumnWidthCount = ( time1 - time2 ) / povDisplayColumnCount;    // Calculate the duration for a column in uSeconds
  //povDisplayColumn=0;                           // Reset display to the first column keeps video vertical prevents drift

}