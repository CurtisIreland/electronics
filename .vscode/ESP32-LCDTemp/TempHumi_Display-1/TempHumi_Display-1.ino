/*******************************************************************
 This is the first part of the Basement Electronics project where
 we display temperature and humidity on a round 1.28" LCD screen 
 with an attached ESP32-S3 microcontroller.

 This code is incomplete and will be finished in the next part.

 Project is recorded on my GitHub: 
   https://github.com/CurtisIreland/electronics/ESP32-LCDTemp

 All code is licensed under Creative Commons
 Video pubish date: October 19, 2024
********************************************************************/

#define DIAL_CENTRE_X 120
#define DIAL_CENTRE_Y 120
#define TRING_RADIUS 90
#define HRING_RADIUS 120
#define RING_WIDTH 30

// Font attached to this sketch
#include "ArialBoldMT28.h"
#define AA_FONT_LARGE ArialBoldMT28

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

uint16_t t_width = 0;
uint16_t h_width = 0;
uint16_t bg_color = 0;

void setup() {
  Serial.begin(115200);  // Debug only

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, TRING_RADIUS - (RING_WIDTH + 1), TFT_DARKGREY);

  // Plot the label text
  tft.setTextColor(TFT_WHITE, bg_color);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

  // Load the font and create the Sprite for reporting the value
  tft.loadFont(AA_FONT_LARGE);
  t_width = tft.textWidth("-77.7C");   // 7 is widest numeral in this font
  h_width = tft.textWidth("777%");     // 7 is widest numeral in this font
  bg_color = tft.readPixel(120, 120);  // Get colour from dial centre

  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, HRING_RADIUS, HRING_RADIUS-RING_WIDTH, 45, 315, TFT_BLUE, TFT_DARKGREY, true);
  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, TRING_RADIUS, TRING_RADIUS-RING_WIDTH, 45, 315, TFT_RED, TFT_DARKGREY, true);

  tft.drawNumber(0, DIAL_CENTRE_X, DIAL_CENTRE_Y - tft.fontHeight());
  tft.drawNumber(0, DIAL_CENTRE_X, DIAL_CENTRE_Y);

  // Define where the needle pivot point is on the TFT before
  // creating the needle so boundary calculation is correct
//  tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);

  // Create the needle Sprite
//  createNeedle();

  // Reset needle position to 0
  plotValue(0.0, 0.0);

  delay(2000);
}

// =======================================================================================
// Loop
// =======================================================================================
void loop() {
  // Plot needle at random angle in range 0 to 240, speed 40ms per increment
  plotValue(0.0, 0.0);

  // Pause at new position
  delay(2500);
}


// =======================================================================================
// Create the needle Sprite
// =======================================================================================
//void createNeedle(void)
//{
//  needle.setColorDepth(16);
//  needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite
//
//  needle.fillSprite(TFT_BLACK); // Fill with black
//
//  // Define needle pivot point relative to top left corner of Sprite
//  uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)
//  uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
//  needle.setPivot(piv_x, piv_y);     // Set pivot point in this Sprite
//
//  // Draw the red needle in the Sprite
//  needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
//  needle.fillRect(1, 1, NEEDLE_WIDTH-2, NEEDLE_LENGTH-2, TFT_RED);
//
//  // Bounding box parameters to be populated
//  int16_t min_x;
//  int16_t min_y;
//  int16_t max_x;
//  int16_t max_y;
//
//  // Work out the worst case area that must be grabbed from the TFT,
//  // this is at a 45 degree rotation
//  needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);
//
//  // Calculate the size and allocate the buffer for the grabbed TFT area
//  tft_buffer =  (uint16_t*) malloc( ((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2 );
//}

// =======================================================================================
// Move the needle to a new position
// =======================================================================================
void plotValue(float read_temp, float read_humi) {
    // Update the number at the centre of the dial
//    spr.setTextColor(TFT_WHITE, bg_color, true);
//    spr.drawNumber(old_angle+120, spr_width/2, spr.fontHeight()/2);
//    spr.pushSprite(120 - spr_width / 2, 120 - spr.fontHeight() / 2);

}

// =======================================================================================
