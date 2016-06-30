#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Colour and position variables
int red_value = 255;
int green_value = 255;
int blue_value = 255;
int red_position = 50;
int green_position = 50;
int blue_position = 50;
int old_position = 0;
int fill_color = tft.color565(red_value, green_value, blue_value);

// LED Variables
boolean led_state = true;
int led_r_pin = 3;
int led_g_pin = 5;
int led_b_pin = 6;

void setup() {
  // Start our TFT
  tft.begin();

  / Start our touchscreen
  if (!ts.begin()) {
    while (1);
  }

  // Initialize our RGB LED
  pinMode(led_r_pin, OUTPUT);
  pinMode(led_g_pin, OUTPUT);
  pinMode(led_b_pin, OUTPUT);
  analogWrite(led_r_pin, red_value);
  analogWrite(led_g_pin, green_value);
  analogWrite(led_b_pin, blue_value);

  draw_screen();
}

void loop() {
  // See if there's any  touch data for us
  if (ts.bufferEmpty()) {
    return;
  }

  // Retrieve a point  
  TS_Point p = ts.getPoint();

  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  // Check sliders
  if (p.y > 50 && p.y < 230) {
    // Red Slider
    if (p.x > 40 && p.x < 80) {
      // Map slider y value to LED intensity
      red_value = map(p.y, 230, 50, 0, 255);

      // Grab new slider position after storing the old one
      old_position = red_position;
      red_position = p.y;

      // Black out the old slider, re-draw the verticle bar, then draw the new slider
      tft.drawFastHLine(40, old_position, 40, ILI9341_BLACK);
      tft.drawPixel(60, old_position, ILI9341_WHITE);
      tft.drawFastHLine(40, red_position, 40, ILI9341_WHITE);
    }
    // Green Slider
    if (p.x > 100 && p.x < 140) {
      // Map slider y value to LED intensity
      green_value = map(p.y, 230, 50, 0, 255);

      // Grab new slider position after storing the old one
      old_position = green_position;
      green_position = p.y;

      // Black out the old slider, re-draw the verticle bar, then draw the new slider
      tft.drawFastHLine(100, old_position, 40, ILI9341_BLACK);
      tft.drawPixel(120, old_position, ILI9341_WHITE);
      tft.drawFastHLine(100, green_position, 40, ILI9341_WHITE);
    }
    // Blue Slider
    if (p.x > 160 && p.x < 200) {
      // Map slider y value to LED intensity
      blue_value = map(p.y, 230, 50, 0, 255);

      // Grab new slider position after storing the old one
      old_position = blue_position;
      blue_position = p.y;

      // Black out the old slider, re-draw the verticle bar, then draw the new slider
      tft.drawFastHLine(160, old_position, 40, ILI9341_BLACK);
      tft.drawPixel(180, old_position, ILI9341_WHITE);
      tft.drawFastHLine(160, blue_position, 40, ILI9341_WHITE);
    }
  }

  // Toggle the LED state (on/off)
  if ((p.x > 60 && p.x < 180) && (p.y > 270 && p.y < 310)) {
    led_state = ! led_state;
    delay(250);
  }

  // Set the LED colour and fill colour based on the slider positions
  if (led_state) {
    fill_color = tft.color565(red_value, green_value, blue_value);
    analogWrite(led_r_pin, red_value);
    analogWrite(led_g_pin, green_value);
    analogWrite(led_b_pin, blue_value);
  } else {
    fill_color = ILI9341_BLACK;
    analogWrite(led_r_pin, 0);
    analogWrite(led_g_pin, 0);
    analogWrite(led_b_pin, 0);
  }
  
  // Redraw LED Status Button
  tft.fillRect(60, 270, 120, 40, fill_color);
  tft.drawRect(60, 270, 120, 40, ILI9341_WHITE);
}

void draw_screen() {
  // Create a blank, black screen
  tft.fillScreen(ILI9341_BLACK);

  // Vertical slider lines and labels
  tft.fillCircle(60, 20, 10, ILI9341_RED);
  tft.fillCircle(120, 20, 10, ILI9341_GREEN);
  tft.fillCircle(180, 20, 10, ILI9341_BLUE);
  tft.drawFastVLine(60, 50, 180, ILI9341_WHITE);
  tft.drawFastVLine(120, 50, 180, ILI9341_WHITE);
  tft.drawFastVLine(180, 50, 180, ILI9341_WHITE);
  tft.drawFastHLine(40, red_position, 40, ILI9341_WHITE);
  tft.drawFastHLine(100, green_position, 40, ILI9341_WHITE);
  tft.drawFastHLine(160, blue_position, 40, ILI9341_WHITE);

  // LED Status Button
  tft.fillRect(60, 270, 120, 40, fill_color);
  tft.drawRect(60, 270, 120, 40, ILI9341_WHITE);
}

