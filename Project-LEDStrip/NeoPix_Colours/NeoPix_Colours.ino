// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 30

uint32_t rgb_colours[3];
uint8_t cur_colour = 0;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

//  rgb_colours[0] = strip.Color(255, 255, 255);
  rgb_colours[0] = strip.Color(127, 127, 127);
  rgb_colours[1] = strip.Color(127, 0, 0);
  rgb_colours[2] = strip.Color(0, 127, 0);
}

void loop() {
//  colorWipe(rgb_colours[cur_colour], 50); 
//  theaterChase(rgb_colours[cur_colour], 50);
//  cur_colour = (cur_colour + 1) % 3;

  colourAlt(rgb_colours, 250);
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, strip.gamma32(color));         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  delay(wait * strip.numPixels());                           //  Pause for a moment
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}


// colourAlt - alternate colours within the array
void colourAlt(uint32_t *colours, int wait) {
  int loop_times = 10;
//  int num_colours = sizeof(colours) / sizeof(colours[0]);
  int num_colours = 3;
  int col_current = 0;
  int col_start = 0;
  Serial.print("Number of colours: ");
  Serial.println(num_colours);

  for(int a=0; a < (loop_times * num_colours); a++) {
    col_current = col_start;
    for(int b=0; b < strip.numPixels(); b++) {
      strip.setPixelColor(b, colours[col_current]);
      col_current = (col_current + 1) % num_colours;
    }
    strip.show(); // Update strip with new contents
    col_start = (col_start + 1) % num_colours;
    delay(wait);  // Pause for a moment
  }
}
