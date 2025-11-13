/*
  ESP-NOW Data Display - Receiver
  ESPNOW-Dispaly.ino
  Receives Temperature & PRessure data from other ESP32 via ESP-NOW
  
  Inspired by DroneBot Workshop
  https://dronebotworkshop.com
*/

// Include required libraries
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SSD1315.h>

// OLED display instance
SSD1315 display;
#define SDA_PIN 5
#define SCK_PIN 6
#define I2C_FREQ 240000000

// Define data structure
typedef struct struct_message {
    float temp;
    float press;
    int ident;
} struct_message;

// Create structured data object
struct_message myData;

// Callback function
void OnDataRecv(const esp_now_recv_info_t *mac, const uint8_t *incomingData, int len)
{
  char temp_str[16];
  char press_str[16];

  // Format incoming data into structure
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Format test for OLED display
  sprintf(temp_str, "%d:%.1fC", myData.ident, myData.temp);
  sprintf(press_str, "%d:%.1fhPa", myData.ident, myData.press);

  // Show data on display
  display.drawRect(0, 0, 71, 39, true);          // filled area
  display.drawString(0, 5, temp_str, 12, true);  // inverted text
  display.drawString(0, 22, press_str, 12, true);  // inverted text
  display.display();

  // Print to Serial Monitor
  Serial.print("Temp Sensor ");
  Serial.print(myData.ident);
  Serial.print(": ");
  Serial.println(myData.temp);
    
  Serial.print("Pressure Sensor ");
  Serial.print(myData.ident);
  Serial.print(": ");
  Serial.println(myData.press); 

  Serial.println("");
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set up the OLED display
  Wire.begin(SDA_PIN, SCK_PIN, I2C_FREQ);
  display.begin();
  display.setRotation(2);       // rotate 180 degrees
  display.setBrightness(0x7F);  // medium brightness

  // Set initial display text
  display.clear();
  display.drawString(15, 5, "ESP-NOW", 12, false);
  display.drawString(15, 22, "Display", 12, false);
  display.display();

  // Start ESP32 in Station mode
  WiFi.mode(WIFI_STA);

  // Initalize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  }