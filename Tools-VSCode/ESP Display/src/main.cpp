#include <Wire.h>
#include <SSD1315.h>

// OLED display instance, reset pin connected to default 8
SSD1315 display;
#define SDA_PIN 5
#define SCK_PIN 6

void setup() {
  Wire.begin(SDA_PIN, SCK_PIN);
  display.begin();
  display.setRotation(0);       // rotate 90 degrees
  display.setBrightness(0x7F);  // medium brightness

  // draw initial bitmap image
  display.clear();
  display.drawBitmap(0, 0, PIC1, 72, 40);
  display.display();
  delay(1500);

  // show inverted second image
  display.clear();
  display.drawBitmap(0, 0, PIC2, 72, 40);
  display.display();
  delay(1500);

  // draw UI icons across the top
  display.clear();
  display.drawBitmap(0, 0, Signal816, 16, 8);    // signal strength icon
  display.drawBitmap(24, 0, Bluetooth88, 8, 8);  // bluetooth icon
  display.drawBitmap(40, 0, Msg816, 16, 8);      // message icon
  display.drawBitmap(64, 0, GPRS88, 8, 8);       // network icon

  // print a short status text
  display.drawString(0, 10, "SSD1315 Demo");

  // draw a filled rectangle around the bottom area
  display.drawRect(0, 24, 71, 39, true);          // filled area
  display.drawString(2, 28, "Hello!", 12, true);  // inverted text
  display.display();
  delay(2000);

  // pixel test pattern
  display.clear();
  for (int x = 0; x < 72; x += 2) {
    for (int y = 0; y < 40; y += 2) {
      display.drawPixel(x, y, 1);
    }
  }
  display.display();
  delay(2000);

  // rotate display 180 degrees
  display.setRotation(2);
  display.clear();
  display.drawString(0, 0, "Rotated", 5);
  display.display();
  delay(1500);

  // show a large character using 16x8 font
  display.clear();
  display.drawChar(28, 12, 'A', 16);  // center of screen
  display.display();
  delay(1500);

  // demonstrate small 5x8 font
  display.clear();
  display.drawString(0, 0, "Small text", 5);
  display.display();
  delay(1500);

  // rotate display 270 degrees
  display.setRotation(3);
  display.clear();
  display.drawString(0, 0, "270 deg", 5);
  display.display();
  delay(1500);

  // dim display then go to sleep
  display.setBrightness(0x10);
  delay(500);
  display.sleep(true);
  delay(1000);
  display.sleep(false);
  display.setBrightness(0x7F);
  display.setRotation(0);
  display.clear();
  for (int x = 0; x < 72; x += 2) {
    for (int y = 0; y < 40; y += 2) {
      display.drawPixel(x, y, 1);
    }
  }
  display.drawString(0, 10, "SSD1315 Demo");
  display.display();
  while (true) {
    for (int b = 0x00; b < 0xFF; b += 2) {
      display.setBrightness(b);
      delay(20);
    }
    for (int b = 0xFF; b > 0x00; b -= 2) {
      display.setBrightness(b);
      delay(20);
    }
  }
}

void loop() {}