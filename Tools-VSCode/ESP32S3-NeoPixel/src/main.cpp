#include <Arduino.h>

#define RGB_BRIGHTNESS 8

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  neopixelWrite(RGB_BUILTIN, 255, 0, 0);
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, 255, 0);
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, 0, 255);
  delay(1000);
}
