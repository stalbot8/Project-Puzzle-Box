/* 
  Example "Protest scroller" for 8x32 WS2812 "NeoPixel Display
  Assumes you are running a standard Arduino ATMega328 compatible board
*/


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// DATA PIN
// If you want to use a different data pin on your microcontroller CHANGE THIS!
#define DATA_PIN 6

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

// Matrix setup params
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(24, 8, DATA_PIN,
NEO_MATRIX_TOP    + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE + NEO_MATRIX_ZIGZAG,
NEO_GRB            + NEO_KHZ800);


// Edit this
// The important stuff, your message and colors

char* Text[]= {"WE'RE PREGNANT!"};
const uint16_t colors[] = {
  matrix.Color(255, 255, 255)};
int brightness = 20;
// End Edit

int numMode = arr_len(Text)-1;
int numColor = arr_len(colors)-1;
int pixelPerChar = 4;
int maxDisplacement;
int mode = 0;

void setup() { 
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
  matrix.setTextColor(colors[0]);
  Serial.begin(9600);
}

int y = matrix.height();
int x = matrix.width();
int pass = 0;
int line_pass = 0;

void loop() {
    
  // if mode greater than numMode reset
  if (mode > numMode) { mode = 0; }

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  scroll(Text[mode],75);
  
}

// this does the magic of scrolling
void scroll(char* message,int delays) {
  maxDisplacement = strlen(message) * pixelPerChar + matrix.width();
  if(++line_pass > matrix.width()) line_pass = 0;
  matrix.print(String(message));
  if(--x < -maxDisplacement) {
    x = matrix.width();
    if(++pass >= numColor) { pass = 0;  };
    matrix.setTextColor(colors[pass]);
    mode++;
  }
  matrix.show();
  delay(delays);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<294; i++) {
      matrix.drawPixel(0, i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
