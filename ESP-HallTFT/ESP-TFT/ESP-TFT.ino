#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

char sensorPrintout[4];

void setup() {
  tft.init();
  tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);

  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE);
//  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.print("Sensor Value : ");

  tft.setTextSize(5);
}

void loop() {
  String sensorVal = String(hallRead());
  sensorVal.toCharArray(sensorPrintout, 4);
  
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 15);
  tft.print(sensorPrintout);

  delay(1000);
}
