/*
  ESP-NOW Sensor - Transmitter
  ESPNOW_Sensor.ino
  Sends Temperature & Pressure data to other ESP32 via ESP-NOW
  Uses BMP280
  
  Inspired by DroneBot Workshop
  https://dronebotworkshop.com
*/

// Include required libraries
#include <WiFi.h>
#include <esp_now.h>
#include <BME280I2C.h>
#include <Wire.h>

// Variables for temperature and pressure
BME280I2C bme;
float temp;
float humid;
float press;

// Integer for identification (make unique for each transmitter)
int ident = 2;

// Responder MAC Address (Replace with your responders MAC Address)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Define data structure
typedef struct struct_message {
  float temp;
  float humid;
  float press;
  int ident;
} struct_message;

// Create structured data object
struct_message myData;

// Register peer
esp_now_peer_info_t peerInfo;

// Sent data callback function
void OnDataSent(const wifi_tx_info_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  // Setup Serial monitor
  Serial.begin(115200);
  delay(100);

  // Configure BMP280 for I2C bus
  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  // Set ESP32 WiFi mode to Station temporarly
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Define callback
  esp_now_register_send_cb(OnDataSent);


  // Save the receiver MAC address in our peer list for transmit
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0; // Set our Wi-Fi channel
  peerInfo.encrypt = false; // Disable link encryption

  // Make connection to receiver (peer)
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

}

void loop() {

  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Pressure: ");
  Serial.println(pres);

  // Add to structured data object
  myData.temp = temp;
  myData.humid = hum;
  myData.press = pres;
  myData.ident = ident;

  // Send data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  // Delay for sensor between 2 to 6 seconds
  delay(random(2,6) * 1000);
}