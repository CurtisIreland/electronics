#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

// Font attached to this sketch
#include "ArialBoldMT52.h"
#define AA_FONT_LARGE ArialBoldMT52
#include "ArialBlack24.h"
#define AA_FONT_MEDIUM ArialBlack24


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times

void setup(void) {
  tft.init();
  tft.setRotation(3);
  tft.loadFont(AA_FONT_LARGE);

//  tft.setTextSize(4);
  tft.fillScreen(TFT_BLACK);
}
  
// Set X and Y coordinates for center of display
//  int centerX = SCREEN_WIDTH / 2;
//  int centerY = SCREEN_HEIGHT / 2;
//  tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);

void loop() {
  tft.setTextColor(TFT_GREEN);
  tft.drawString("E", 45, 45);

  tft.setTextColor(TFT_BLUE);
  tft.drawString("B", 20, 20);

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  tft.setTextColor(TFT_WHITE);
  tft.loadFont(AA_FONT_MEDIUM);
  tft.drawCentreString("Cheap Yellow Display", centerX, centerY, 2);

  while(true);
}
