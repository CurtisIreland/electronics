#include <SPI.h>
#include <WiFiNINA.h>

void setup() {
  Serial.begin(9600);
  WiFi.WiFiInitLEDs();
}

void loop() {

  for(int i = 0; i < 256; i++) {
    WiFi.WiFiRedLED(i);
    WiFi.WiFiBlueLED(255-i);
    Serial.print("Red: ");
    Serial.println(i);
    Serial.print("Blue: ");
    Serial.println(255 - i);
    delay(100);
  }
  for(int i = 0; i < 256; i++) {
    WiFi.WiFiGreenLED(i);
    WiFi.WiFiRedLED(255-i);
    Serial.print("Green: ");
    Serial.println(i);
    Serial.print("Red: ");
    Serial.println(255 - i);
    delay(100);
  }
  for(int i = 0; i < 256; i++) {
    WiFi.WiFiBlueLED(i);
    WiFi.WiFiGreenLED(255-i);
    Serial.print("Blue: ");
    Serial.println(i);
    Serial.print("Green: ");
    Serial.println(255 - i);
    delay(100);
  }
}
