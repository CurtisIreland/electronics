/*******************************************************************
 This is the second part of the Basement Electronics project where
 we display temperature and humidity on a round 1.28" LCD screen 
 with an attached ESP32-S3 microcontroller.

 Project is recorded on my GitHub: 
   https://github.com/CurtisIreland/electronics/ESP32-LCDTemp

 All code is licensed under Creative Commons
 Video pubish date: October 26, 2024
********************************************************************/

#include <AM2302-Sensor.h>

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


constexpr unsigned int SENSOR_PIN {15U};

AM2302::AM2302_Sensor am2302{SENSOR_PIN};

uint16_t t_width = 0;
uint16_t h_width = 0;
uint16_t bg_color = 0;

int read_val = 0;
bool delta = true;

void setup() {
  Serial.begin(115200);  // Debug only
  // set pin and check for sensor
  if (am2302.begin()) {
    // this delay is needed to receive valid data,
    // when the loop directly read again
    delay(3000);
  }
  else {
    while (true) {
      Serial.println("Error: sensor check. => Please check sensor connection!");
      delay(10000);
    }
  }

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, TRING_RADIUS - (RING_WIDTH + 1), TFT_DARKGREY);

  // Plot the label text
  tft.setTextColor(TFT_WHITE, bg_color);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 75, 2);
  tft.drawString("(humidity)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 100, 2);

  // Load the font and create the Sprite for reporting the value
  tft.loadFont(AA_FONT_LARGE);
  t_width = tft.textWidth("-77.7C");   // 7 is widest numeral in this font
  h_width = tft.textWidth("177.7%");     // 7 is widest numeral in this font
  bg_color = tft.readPixel(120, 120);  // Get colour from dial centre

  // Reset needle position to 0
  plotValue(0.0, 0.0);
}

// =======================================================================================
// Loop
// =======================================================================================
void loop() {
  float read_temp = 0.0;
  float read_humi = 0.0;

  auto status = am2302.read();
  Serial.print("\n\nstatus of sensor read(): ");
  Serial.println(status);

  read_temp = am2302.get_Temperature();
  read_humi = am2302.get_Humidity();

  Serial.println(read_temp);
  Serial.println(read_humi);

/** Value test ** /
  if(delta) {
    read_val++;
  } else {
    read_val--;
  }
  if((read_val > 24) or (read_val < 1)) delta = !delta;
  read_temp = (read_val * 2.8) - 10.0;
  read_humi = (read_val * 4.0);
/**  **/
  plotValue(read_temp, read_humi);

  // Pause at new position
  delay(5000);
}

void plotValue(float read_temp, float read_humi) {
  uint32_t tmap = map((long) read_temp, -10, 60, 45, 315);
  uint32_t hmap = map((long) read_humi, 0, 100, 45, 315);

  String temp_str = String(read_temp, 1) + "C";
  String humi_str = String(read_humi, 1) + "%";

  Serial.println("Temperature: " + temp_str);
  Serial.println("Humidity: " + humi_str);

  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, HRING_RADIUS, HRING_RADIUS-RING_WIDTH, 45, 316, bg_color, bg_color, true);
  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, HRING_RADIUS, HRING_RADIUS-RING_WIDTH, 45, hmap + 1, TFT_BLUE, TFT_DARKGREY, true);
   
  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, TRING_RADIUS, TRING_RADIUS-RING_WIDTH, 45, 316, bg_color, bg_color, true);
  tft.drawSmoothArc(DIAL_CENTRE_X, DIAL_CENTRE_Y, TRING_RADIUS, TRING_RADIUS-RING_WIDTH, 45, tmap + 1, TFT_RED, TFT_DARKGREY, true);

  tft.loadFont(AA_FONT_LARGE);
  tft.setTextPadding(t_width);
  tft.drawString(temp_str, DIAL_CENTRE_X, DIAL_CENTRE_Y - tft.fontHeight());

  tft.setTextPadding(h_width);
  tft.drawString(humi_str, DIAL_CENTRE_X, DIAL_CENTRE_Y);
  tft.unloadFont();
}

// =======================================================================================
