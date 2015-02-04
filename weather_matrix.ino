/*
*  This code goes a spark.io core, and is part of a #MakeEveryWeek  
*  project by John Keefe. More info at http://johnkeefe.net
* 
*  Details on this project at 
*    http://johnkeefe.net/make-every-week-entryway-weatherbot
* 
*/

// The next three include lines call in the libraries. To learn how
// to add them to this project on the spark_io core, see:
// http://johnkeefe.net/make-every-week-entryway-weatherbot

// This #include statement was automatically added by the Spark IDE.
#include "neopixel/neopixel.h"

// This #include statement was automatically added by the Spark IDE.
#include "neomatrix/neomatrix.h"

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GFX/Adafruit_GFX.h"

#include "application.h"

// IMPORTANT: Set pixel PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B

// Carrying over the comments from the NEOMATRIX example application ...
// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.
// Parameter 7 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
//   NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
//   For Spark Core developement it should probably also be WS2812B if you're
//   using adafruit neopixels.

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, 1, 1, PIXEL_PIN,
  NEO_MATRIX_TOP    + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS +   NEO_MATRIX_PROGRESSIVE + 
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE,
  PIXEL_TYPE);

// declare color variables
// Thanks to Corey Jones's cool example at 
// http://www.hackster.io/jones_corey/christmas-lights-neopixel-led-matrix-glass-block-wall
uint32_t PEACH = matrix.Color(221,171,127);
uint32_t ROSY = matrix.Color(255,176,193);
uint32_t LIGHTBLUE = matrix.Color(94,185,247);
uint32_t RED = matrix.Color(255,0,0);
uint32_t GREEN = matrix.Color(0,255,0);
uint32_t GRAY = matrix.Color(150,150,150);
uint32_t BROWN = matrix.Color(117,76,41);
uint32_t PURPLE = matrix.Color(152,5,229);
uint32_t DARKGREEN = matrix.Color(12,158,17);
uint32_t PINK = matrix.Color(250,101,148);
uint32_t ORANGE = matrix.Color(200,90,41);
uint32_t YELLOW = matrix.Color(255,242,0);
uint32_t BLACK = matrix.Color(0,0,0);
uint32_t WHITE = matrix.Color(255,255,255);
uint32_t BLUE = matrix.Color(0,0,255);
uint32_t AQUA = matrix.Color(2,132,130);
uint32_t DARKBLUE = matrix.Color(0,0,125);

// Variable declaration ...
String icon;         // 3 characters indicating forecast icon
int temp = 0;        // the forecast temperature
int tens_digit;      // the tens digit to display
int ones_digit;      // the ones digit to display
uint32_t text_color; // color for the text

// the setup section runs once
void setup() {

  matrix.begin();
  matrix.setBrightness(60);
  
  // expose the "forecast" component to the api so it responds to
  //   https://api.spark.io/v1/devices/DEVICE_ID/forecast
  // and call the parseForecast function when you see it
  Spark.function("forecast", parseForecast);

  matrix.fillScreen(0); // clears the screen

}

// the loop section repeats forever
void loop() {
    
  // SHOW THE WEATHER ICON
  
  // clear the screen
  matrix.fillScreen(0);
  
  // match the draw functions to the value for icon 
  // picked up from the server program
  if (icon == "sno") { drawSnow(); }
  if (icon == "rai") { drawRain(); }
  if (icon == "cle") { drawSun(); }
  if (icon == "clo") { drawCloudy(); }
  if (icon == "sle") { drawSleet(); }
  if (icon == "fog") { drawFog(); }
  if (icon == "win") { drawWind(); }
  if (icon == "par") { drawPartlyCloudy(); }
  // draws nothing if icon is empty or doesn't match

  matrix.show(); // light it up
  delay(2000); // wait 2 seconds
  

  // SHOW THE TEMPERATURE
  
  // clear the screen
  matrix.fillScreen(0);
  
  // set the text color of the temperature based on its value
  if (temp >= 90 ) { text_color = RED; }
  else if (temp >= 80 ) { text_color = ORANGE; }
  else if (temp >= 70 ) { text_color = YELLOW; }
  else if (temp >= 50 ) { text_color = GREEN; }
  else if (temp >= 32 ) { text_color = AQUA; }
  else if (temp >= 10 ) { text_color = GRAY; }
  else if (temp >= 0 ) { text_color = PINK; }
  else if (temp < 0) { text_color = BLUE; }

  // calculate and draw the ones digit
  ones_digit = temp % 10; // temp modulo 10 ... so 24 becomes 4
  drawDigit(ones_digit, 5, text_color); // send the digit, the LED column and the color
  
  // calculate and draw the tens digit,
  // provided temp is at greater than 10 or less than -10
  if (temp >= 10 || temp <= -10) {
    tens_digit = temp / 10; // all are integers, so 24 becomes 2
    drawDigit(tens_digit, 1, text_color); // send the digit, the LED column and the color
  }
  
  matrix.show(); // light it up
  delay(2000); // wait 2 seconds
  
}

int parseForecast(String command){
    
    // the "command" will come in as a string like these:
    //   -3 snow
    //   89 partly-cloudy-day
    //   103clear-day
    
    // grab the temperature from the first three characters
    temp = command.substring(0,3).toInt();
    
    // grab the icon from the next three characters
    // note: we're always showing a day forecast ... today's if 
    // it is before noon, tomorrow's if after that. so we can 
    // ignore the night icons.
    String sentIcon = command.substring(3,6);

    // update the icon variable with the first three letters of the value sent
    
    icon = sentIcon;
    
    // int fuctions need a return 
    return 1;
    
}


// This function draws digits based on the passed value of "digit", 
// the y (column) position, and the color.
void drawDigit(int digit, int y, uint32_t textcolor) {
    
    bool negative = false;
    
    // if the temp is negative, note that and turn into a positive digit
    if (digit < 0) {
        digit = abs(digit);
        negative = true;
    }
    
    // this picks a draw function based on the value of digit.
    // if there is a more efficient way to do this, I'd love to hear it.
    switch (digit) {
        case 1:
            drawOne(y, textcolor);
            break;
        case 2:
            drawTwo(y, textcolor);
            break;
        case 3:
            drawThree(y, textcolor);
            break;
        case 4:
            drawFour(y, textcolor);
            break;
        case 5:
            drawFive(y, textcolor);
            break;
        case 6:
            drawSix(y, textcolor);
            break;
        case 7:
            drawSeven(y, textcolor);
            break;
        case 8:
            drawEight(y, textcolor);
            break;
        case 9:
            drawNine(y, textcolor);
            break;
        case 0:
            drawZero(y, textcolor);
            break;
        // handle the passing of a 10 or 11 in the
        // tens postion for 100+ temperatures
        case 10: 
            drawZero(y, textcolor);
            break;
        case 11:
            drawOne(y, textcolor);
            break;
            
    }

    // draw a white minus sign for negative numbers
    if (negative == true){
        matrix.drawLine( 2,0, 2,1, WHITE);
    }
            
    
}

// FUNCIONS FOR DRAWING ICONS
// The defitions for drawLine, drawPixel, drawRect, etc are here:
//  https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
void drawSnow() {
    matrix.drawLine( 0,1, 0,6, BLUE);  // x1,y1, x2,y2, color
    matrix.drawLine( 1,0, 1,7, BLUE);
    matrix.drawLine( 2,1, 2,6, BLUE);
    matrix.drawPixel( 3,2, WHITE); // x1,y1, color
    matrix.drawPixel( 3,4, WHITE);
    matrix.drawPixel( 3,6, WHITE);
    matrix.drawPixel( 4,1, WHITE);
    matrix.drawPixel( 4,3, WHITE);
    matrix.drawPixel( 4,5, WHITE);
}

void drawSun() {
    matrix.drawLine( 0,1, 4,5, YELLOW);
    matrix.drawLine( 0,5, 4,1, YELLOW);
    matrix.drawLine( 0,3, 4,3, YELLOW);
    matrix.drawLine( 2,0, 2,6, YELLOW);
    
}

void drawRain() {
    matrix.drawLine( 0,1, 0,6, DARKBLUE);  // x1,y1, x2,y2, color
    matrix.drawLine( 1,0, 1,7, DARKBLUE);
    matrix.drawLine( 2,1, 2,6, DARKBLUE);
    matrix.drawPixel( 3,2, BLUE);
    matrix.drawPixel( 3,4, BLUE);
    matrix.drawPixel( 3,6, BLUE);
    matrix.drawPixel( 4,1, BLUE);
    matrix.drawPixel( 4,3, BLUE);
    matrix.drawPixel( 4,5, BLUE);
}

void drawCloudy() {
    matrix.drawLine( 0,1, 0,6, DARKBLUE);  // x1,y1, x2,y2, color
    matrix.drawLine( 1,0, 1,7, DARKBLUE);
    matrix.drawLine( 2,1, 2,6, DARKBLUE);
}

void drawPartlyCloudy() {
    matrix.drawLine( 0,4, 0,6, YELLOW);
    matrix.drawLine( 1,5, 1,6, YELLOW);
    matrix.drawPixel( 2,6, YELLOW);
    matrix.fillRect( 1,1, 4, 4, BLUE);  // x0,y0, h, w, color
    matrix.drawLine( 2,0, 3,0, BLUE);
    matrix.drawLine( 2,5, 3,5, BLUE);
}

void drawFog() {
    matrix.fillScreen(GRAY);
    matrix.drawPixel( 0,0, BLACK); 
    matrix.drawPixel( 0,7, BLACK);
    matrix.drawPixel( 4,0, BLACK);
    matrix.drawPixel( 4,7, BLACK);
    matrix.drawLine( 2,0, 2,2, DARKBLUE);
    matrix.drawLine( 1,4, 1,6, DARKBLUE);
    matrix.drawLine( 3,6, 3,7, DARKBLUE);
}

void drawSleet() {
    matrix.drawLine( 1,0, 3,0, AQUA);
    matrix.drawLine( 0,2, 2,2, WHITE);
    matrix.drawLine( 1,4, 3,4, AQUA);
    matrix.drawLine( 0,6, 2,6, WHITE);
}

void drawWind() {
    matrix.drawLine( 1,0, 0,1, BLUE);
    matrix.drawLine( 4,0, 1,2, BLUE);
    matrix.drawPixel( 1,3, BLUE);
    matrix.drawLine( 3,3, 2,4, BLUE);
    matrix.drawLine( 2,6, 1,7, BLUE);
    matrix.drawLine( 4,4, 4,5, BLUE);
}

// FUNCTIONS FOR DRAWING THE ACTUAL NUMBERS

// each of the digit functions get passed
// the LED matrix column to start in 
// (which could be 0-7, but we only use 1 and 5)
// and the color to use. 
void drawOne(int y, uint32_t textcolor) {
    matrix.drawLine( 0,y+2, 4,y+2, textcolor ); // y is the upper-left corner's column
}

void drawTwo(int y, uint32_t textcolor) {
    matrix.fillRect( 0,y, 5, 3, textcolor); // make a box ... x,y, h, w
    matrix.drawLine( 1,y, 1,y+1, BLACK); // knock out a piece
    matrix.drawLine( 3,y+1, 3,y+2, BLACK);
}

void drawThree(int y, uint32_t textcolor) {
    matrix.fillRect (0,y, 5,3, textcolor);
    matrix.drawLine( 1,y, 1,y+1, BLACK);
    matrix.drawLine( 3,y, 3,y+1, BLACK);
}

void drawFour(int y, uint32_t textcolor) {
    matrix.fillRect( 0,y, 5, 3, textcolor);
    matrix.drawRect( 3,y, 2, 2, BLACK);
    matrix.drawLine( 0,y+1, 1,y+1, BLACK);
}

void drawFive(int y, uint32_t textcolor) {
    matrix.fillRect( 0,y, 5, 3, textcolor);
    matrix.drawLine( 1,y+2, 1,y+1, BLACK);
    matrix.drawLine( 3,y, 3,y+1, BLACK);
}

void drawSix(int y, uint32_t textcolor){
    matrix.fillRect( 0,y, 5, 3, textcolor);
    matrix.drawLine( 1,y+2, 1,y+1, BLACK);
    matrix.drawPixel( 3,y+1, BLACK);
}

void drawSeven(int y, uint32_t textcolor) {
    matrix.drawLine( 0,y, 0,y+2, textcolor);
    matrix.drawLine( 1,y+2, 4,y+2, textcolor);
}

void drawEight(int y, uint32_t textcolor) {
    matrix.fillRect( 0,y, 5, 3, textcolor);
    matrix.drawPixel( 1,y+1, BLACK);
    matrix.drawPixel( 3,y+1, BLACK);
}

void drawNine(int y, uint32_t textcolor) {
    matrix.fillRect( 0,y, 5, 3, textcolor);
    matrix.drawPixel( 1,y+1, BLACK);
    matrix.drawLine( 3,y, 3,y+1, BLACK);
}

void drawZero(int y, uint32_t textcolor) {
    matrix.drawRect( 0,y, 5, 3, textcolor);
}


