/******************************************
  (C)2026 Basement Electronics/Curtis Ireland
  Licensed under Creative Commons - CC BY-NC-SA 4.0

	Remote node. Collect GPS data and send it to the station 
	node. Compare the two locations and show the distance.
	Use ESP-Now as our communications protocol
 ******************************************/

#include "Arduino.h"
#include "HT_TinyGPS++.h"
#include "HT_st7735.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// Data structure to hold GPS location
typedef struct gps_data {
	double latitude;
	double longitude;
} gps_data;

// Data structuere of the transmission packet
typedef struct data_packet {
	bool mark;
	int counter;
	double latitude;
	double longitude;
} data_packet;

// Common definitions
#define VGNSS_CTRL Vext // GPIO pin to enable/disabe the GPS and display
#define UPDATE_POSITION 3000 // How often do we update our local position? (ms)
#define COMM_INTERVAL 10000 // How long between communications intervals before data is stale (ms)
TinyGPSPlus gps;
HT_st7735 st7735;
gps_data local_position; // Location retreived from the GPS
data_packet myData; // Transmitted data
unsigned long posUpdate = millis(); // Time since last position update
unsigned long lastComm = 0; // Time of last communication
int counter = 0;
bool data_received = true;
bool set_home = false;

// Responder MAC Address (Replace with your responders MAC Address)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Register peer
esp_now_peer_info_t peerInfo;

// Manage Vext state for GPS and display
void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
  
}
void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void LEDON(void) {
	pinMode(LED ,OUTPUT);
	digitalWrite(LED, HIGH);
Profile picture

}

void LEDOFF(void) {
	pinMode(LED ,OUTPUT);
	digitalWrite(LED, LOW);
}

// Interrupt callback fro when GPIO 0 is pressed
void home_interrupt(void) {
	set_home = true;
}

gps_data getGPS() {
	bool gps_read = false;
	gps_data read_data;
	char distance_str[16];
	uint16_t colour = ST7735_BLUE; // Colour for valid location data

  // Read GPS Data
	while(! gps_read)
	{
		if(Serial1.available()>0)
		{
			if(Serial1.peek()!='\n')
			{
				gps.encode(Serial1.read());
			}
			else
			{
				Serial1.read();
				read_data.latitude = gps.location.lat();
				read_data.longitude  = gps.location.lng();

				gps_read = true;
			}
		}
	}

  // Change our status colour if GPS not returning valid data
	if(read_data.latitude == 0.0) {
		colour = ST7735_RED;
	}

	// Flush the GPS serial cache
	while(Serial1.read()>0);

  // Display current cooredinates
  sprintf(distance_str, "%.3f %.3f", read_data.latitude, read_data.longitude);
	st7735.st7735_fill_rectangle(0, 20, 160, 20, colour); // Colour bar to show location status (stale vs. updated)
//	st7735.st7735_fill_rectangle(0, 20, 160, 20, ST7735_BLACK);
//	st7735.st7735_write_str(0, 20, distance_str, Font_11x18, colour); 
//This was for testing. We do not want to actually show our loacation on the display

	return(read_data);
}

// Calculate the distance between two points
double distance_in_m(double start_lat, double start_long, double end_lat, double end_long) {
      start_lat/= 180 / PI; 
      start_long/= 180 / PI;
      end_lat/= 180 / PI;
      end_long/= 180 / PI;
      
      // haversine formula 
      double a = pow(sin((end_lat-start_lat)/2), 2) + cos(start_lat) * cos(end_lat) * pow(sin((end_long-start_long)/2), 2);
      double answer = _GPS_EARTH_MEAN_RADIUS * 2 * atan2(sqrt(a), sqrt(1-a));
      return(answer);
    }

// Received data callback function
void OnDataRecv(const esp_now_recv_info_t *mac, const uint8_t *incomingData, int len)
{
	char distance_str[16];
  // Format incoming data into structure
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Format distance text for display
  sprintf(distance_str, "DIST: %.2f", distance_in_m(local_position.latitude, local_position.longitude, myData.latitude, myData.longitude));

  // Valid data received. Display data accordingly
	LEDON();
	Serial.printf("Received: %d: %.2f %.2f\n", myData.counter, myData.latitude, myData.longitude); 
	st7735.st7735_fill_rectangle(0, 40, 160, 40, ST7735_BLACK);
	st7735.st7735_write_str(0, 40, distance_str, Font_11x18, ST7735_YELLOW);
	st7735.st7735_write_str(0, 60, "Established", Font_11x18, ST7735_GREEN);
	data_received = true;
	lastComm = millis();
}

// Prepare and send data
void transmitData() {
	counter += 1;
  // Add to structured data object
	myData.mark = set_home; // Change the station home position if GPIO 0 is pressed
  myData.counter = counter; // Track our data counter
  myData.latitude = local_position.latitude;
  myData.longitude = local_position.longitude;
	
	set_home = false;

  // Send data using ESP-Now
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

// Sent data callback function
void OnDataSent(const wifi_tx_info_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
	Serial.begin(115200);
	Serial.println("***Starting LoRa Remote ***");
	Serial.println(ESP.getEfuseMac(), HEX);

	// Watch for the buttom press (GPIO 0)
	attachInterrupt(0, home_interrupt, RISING);

  // Enable GPS through GPIO3 (Vext)
  VextON();
	
	// Initialize the display
  delay(100);
  st7735.st7735_init();

  // Establish serial connection to GPS
	Serial1.begin(115200,SERIAL_8N1,33,34);

  // Initialize the output
	st7735.st7735_fill_screen(ST7735_BLACK);
	st7735.st7735_write_str(0, 0, (String)"Location...", Font_11x18, ST7735_GREEN);

	bool found_location = false;
	Serial.println("Establishing local position...");

	while(! found_location) {
		local_position = getGPS();
		if(local_position.latitude > 0.0) {
			Serial.printf("Latitude: %.4f\n", local_position.latitude);
			Serial.printf("Longitude: %.4f\n", local_position.longitude);
		/* This was for testing. We don't really want to show this * /
			st7735.st7735_fill_rectangle(0, 40, 160, 40, ST7735_BLACK);
			String latitude = "LAT: " + (String)local_position.latitude;
			st7735.st7735_write_str(0, 40, latitude);
			String longitude  = "LON: "+  (String)local_position.longitude;
			st7735.st7735_write_str(0, 60, longitude);
		/* */
			found_location = true;
		} else {
			Serial.println("No GPS Signal");
			st7735.st7735_fill_rectangle(0, 40, 160, 20, ST7735_BLACK);
			st7735.st7735_write_str(0, 40, "No signal", Font_11x18, ST7735_RED);
		}
		delay(500);
	}


	Serial.println("*** Start collecting data ***");
	st7735.st7735_fill_screen(ST7735_BLACK);
	st7735.st7735_write_str(0, 0, (String)"Collecting...", Font_11x18, ST7735_CYAN);

  // Start ESP32 WiFi in Station mode
  WiFi.mode(WIFI_AP_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

	// Enable Long Range mode
	esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
	esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_LR); 

  // Register callback functions
  esp_now_register_recv_cb(OnDataRecv);
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
	// Update our current position and send it to the station node
	if(millis() > (posUpdate + UPDATE_POSITION)) {
		Serial.println("Updating local position");
		gps_data new_position = getGPS();
		// Keep the new GPS location if it makes sense
		if(new_position.latitude > 0.0) {
			Serial.println("Position updated");
			local_position = new_position;
		}
		// Send our new data to the station after update
		transmitData();
		posUpdate = millis();
	}

  // Know when we have lost communications with the remote
	if(data_received && (millis() > (lastComm + COMM_INTERVAL))) {
		LEDOFF();
		st7735.st7735_fill_rectangle(0, 60, 160, 20, ST7735_BLACK);
		st7735.st7735_write_str(0, 60, "Lost", Font_11x18, ST7735_RED);
		data_received = false;
	}
}
