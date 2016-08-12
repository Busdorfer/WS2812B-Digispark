#include "FastLED.h"

/*
  Create and fill a Two-Dimensional Array:
              LED-No Bright  OnTime  OffTime cntB  cntOn cntOff  MarkR MarkL   r   g   b
  LED[0][0] - 0      30      50      80      0     0     0       0     0       0   0   0
  LED[0][1] - 1      40      77      40      0     0     0       0     0       0   0   0
  
  for value in array {
    led-update (brightness)
    update-array (all values)
  }
*/

//
// VARIABLES
//
#define NUM_LEDS 5
#define DATA_PIN 0

// Basecolor from https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
// Azure F0FFFF
float baseRed = 0xF0;
float baseGreen = 0xFF;
float baseBlue = 0xFF;

// Define array of leds
CRGB leds[NUM_LEDS];

// Define 2D-Array: Number LEDs and 12 Cols (0-11)
const int rows=NUM_LEDS;
const int cols=12;
int LEDArray[rows][cols];

// Values for random Brightness, Stay On, Stay Off
int MinBright = 40;
int MaxBright = 250;
int MinOn = 10;
int MaxOn = 50;
int MinOff = 10;
int MaxOff = 40;

void createArray(int NoOfLEDs, int FirstLED) {
  // Initialize Array: For every LED save the random-values and set counter to zero
  randomSeed(analogRead(0)); // Should initializes the pseudo-random number generator?!
  for( int x=0; x < NoOfLEDs; x++) {
    int tmpled = FirstLED+x;
    LEDArray[x][0] = tmpled;
    LEDArray[x][1] = random(MinBright, MaxBright);
    LEDArray[x][2] = random(MinOn, MaxOn);
    LEDArray[x][3] = random(MinOff, MaxOff);
    LEDArray[x][4] = 0;
    LEDArray[x][5] = 0;
    LEDArray[x][6] = 0;
    LEDArray[x][7] = 0;
    LEDArray[x][8] = 0;
    LEDArray[x][9] = 0;
    LEDArray[x][10] = 0;
    LEDArray[x][11] = 0;
  }
}

void RunningLights(int Color, int HowOften, int WaveDelay) {
  int Position=0;

  for(int i=0; i<NUM_LEDS*HowOften; i++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        leds[i].r = ((sin(i+Position) * 127 + 128)/255)*Color;
        leds[i].g = ((sin(i+Position) * 127 + 128)/255)*Color;
        leds[i].b = ((sin(i+Position) * 127 + 128)/255)*Color;
      }
      FastLED.show();
      delay(WaveDelay);
  }
}

void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //Unused: LEDS.setBrightness(30); 
  
  //createArray(NUM_LEDS, FirstLED);
  createArray(NUM_LEDS, 0);

  // RunningLights( Color, How Often, Delay );
  RunningLights(200, 10, 30);
}
 
void loop()
{
  for( int CurrLED = 0; CurrLED < NUM_LEDS; CurrLED++)
  {
    int LED           = LEDArray[CurrLED][0];
    int Brightness    = LEDArray[CurrLED][1];
    int OnTime        = LEDArray[CurrLED][2];
    int OffTime       = LEDArray[CurrLED][3];
    int countBright   = LEDArray[CurrLED][4];
    int countOnTime   = LEDArray[CurrLED][5];
    int countOffTime  = LEDArray[CurrLED][6];
    int countdown = LEDArray[CurrLED][7];
    int ledState      = LEDArray[CurrLED][8];
    int lastRed       = LEDArray[CurrLED][9];
    int lastGreen     = LEDArray[CurrLED][10];
    int lastBlue      = LEDArray[CurrLED][11];

    if(ledState==1) {
      countOffTime++;
      LEDArray[CurrLED][6] = countOffTime;
      
      if(countOffTime >= OffTime) {
        //for( int update = 4; update < 8; update++) {
        //  LEDArray[CurrLED][update] = 0;
        //} 
        LEDArray[CurrLED][4]=0;
        LEDArray[CurrLED][5]=0;
        LEDArray[CurrLED][6]=0;
        LEDArray[CurrLED][7]=0;
        LEDArray[CurrLED][8]=0;
        LEDArray[CurrLED][9]=0;
        LEDArray[CurrLED][10]=0;
        LEDArray[CurrLED][11]=0;
      }
    }
    
    if( (countBright <= Brightness) && (countdown==0) )
    {
      // Fade Up
      countBright++;
      lastRed = (countBright/256.0)*baseRed;
      lastGreen = (countBright/256.0)*baseGreen;
      lastBlue = (countBright/256.0)*baseBlue;
      leds[CurrLED].r = lastRed;
      leds[CurrLED].g = lastGreen;
      leds[CurrLED].b = lastBlue;
      LEDArray[CurrLED][4]=countBright;
      LEDArray[CurrLED][9]=lastRed;
      LEDArray[CurrLED][10]=lastGreen;
      LEDArray[CurrLED][11]=lastBlue;
    }
    else if( (countBright >= Brightness) && (countOnTime <= OnTime) && (countdown==0) )
    {
      // Stay ON
      countOnTime++;
      LEDArray[CurrLED][5]=countOnTime;
      if(countOnTime >= OnTime)
      {
        countdown=1;
        LEDArray[CurrLED][7]=countdown;
      }
    }
    else if( (countdown==1) && (countBright>0) )
    {
      // Fade Down
      countBright--;
      lastRed = (countBright/256.0)*baseRed;
      lastGreen = (countBright/256.0)*baseGreen;
      lastBlue = (countBright/256.0)*baseBlue;
      leds[CurrLED].r = lastRed;
      leds[CurrLED].g = lastGreen;
      leds[CurrLED].b = lastBlue;
      LEDArray[CurrLED][4]=countBright;
      LEDArray[CurrLED][9]=lastRed;
      LEDArray[CurrLED][10]=lastGreen;
      LEDArray[CurrLED][11]=lastBlue;
    }
    else
    {
      ledState=1;
      LEDArray[CurrLED][8]=ledState;
    }
    FastLED.show();
    delay(20);
  }
}


