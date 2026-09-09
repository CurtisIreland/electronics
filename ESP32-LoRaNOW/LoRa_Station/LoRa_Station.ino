/******************************************
  (C)2026 Basement Electronics/Curtis Ireland
  Licensed under Creative Commons - CC BY-NC-SA 4.0

	Station node. This node collects data sent by the remote 
	and responds with its location.
	Use LoRa as our communications protocol
 ******************************************/

#include "Arduino.h"
#include "HT_TinyGPS++.h"
#include "HT_st7735.h"
#include "LoRaWan_APP.h"

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

// Enumeration variable to track our current node state
typedef enum {
    LOWPOWER, // Stay in current state. Watch LoRa interrupts
    STATE_RX, // Prepare to receive
    STATE_TX // Prepare to transmit
} States_t;

#define VGNSS_CTRL Vext // GPIO pin to enable/disabe the GPS and display
#define UPDATE_POSITION 10000 // How often do we update our local position? (ms)
#define COMM_INTERVAL 30000 // How long between communications intervals before data is stale (ms)

// LoRa Settings. Station and Remote need to match
#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             5        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       9         // [SF7..SF12]
#define LORA_CODINGRATE                             3         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define RX_TIMEOUT_VALUE                            1000

TinyGPSPlus gps;
HT_st7735 st7735;
gps_data home_position; // Our current location
gps_data local_position; // Location retreived from the GPS
data_packet myData; // Transmitted data
States_t state; // Current node state
unsigned long posUpdate = millis(); // Time since last position update
unsigned long lastComm = 0; // Time of last communication
int counter = 0;
bool data_received = true;
static RadioEvents_t RadioEvents;

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
}

void LEDOFF(void) {
	pinMode(LED ,OUTPUT);
	digitalWrite(LED, LOW);
}

gps_data getGPS() {
	bool gps_read = false;
	gps_data read_data;
	char distance_str[16];
	uint16_t colour = ST7735_BLUE; // Colour for valid location data

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

  // If GPS is not returning valid data, use previously stored values for home
	if(read_data.latitude == 0.0) {
		colour = ST7735_RED; // Indicate this is stale data
		read_data.latitude = home_position.latitude;
		read_data.longitude = home_position.longitude;
	} else {
		home_position.latitude = read_data.latitude;
		home_position.longitude = read_data.longitude;
	}
	/**/

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
void OnRxDone( uint8_t *incomingData, uint16_t len, int16_t rssi, int8_t snr )
{
	char distance_str[16];
  // Format incoming data into structure
  memcpy(&myData, incomingData, sizeof(myData));
	Radio.Sleep( ); // Disable radio for power savings

	// Remote thinks we should use its position as station home
	if(myData.mark) {
		home_position.latitude = myData.latitude;
		home_position.longitude = myData.longitude;
	}

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

  // Change our state to transmit for the next step
	state = STATE_TX;
}

// Prepare and send data
void transmitData() {
	counter += 1;
  // Add to structured data object
  myData.counter = counter; // Track our data counter
  myData.latitude = local_position.latitude;
  myData.longitude = local_position.longitude;
 
  // Send data using LoRa
	Serial.printf("Sending: %d: %.2f %.2f\n", myData.counter, myData.latitude, myData.longitude); 	
	Radio.Send((uint8_t *) &myData, sizeof(myData));
}

// Sent data callback function
void OnTxDone(void)
{
  Serial.print("Packet Successfully sent");
	state = STATE_RX;
}

// Unsuccessful transmit. Try again.
void OnTxTimeout( void )
{
    Serial.println("TX Timeout......");
		Radio.Sleep();
		state = STATE_TX;
}

void setup() {
	Serial.begin(115200);
	Serial.println("*** Starting Station ***");

  // Set our default station location. This is intentionally vague
	home_position.latitude = 44.0;
	home_position.longitude = -76.0;

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

  // Keep looping until GPS finds a location
	while(! found_location) {
		local_position = getGPS();
		if(local_position.latitude > 0.0) {
			Serial.printf("Latitude: %.4f\n", local_position.latitude);
			Serial.printf("Longitude: %.4f\n", local_position.longitude);
		/* This was for testing. We don't really want to show this * /
			st7735.st7735_fill_rectangle(0, 40, 160, 40, ST7735_BLACK);
			String latitude = "LAT: " + (String)local_position.latitude;
			st7735.st7735_write_str(0, 40, latitude);
			String longitude  = "LON: "
Profile picture
NEW SKETCH
+  (String)local_position.longitude;
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

	Serial.println("*** Start listening for remote ***");
	st7735.st7735_fill_screen(ST7735_BLACK);
	st7735.st7735_write_str(0, 0, (String)"Waiting...", Font_11x18, ST7735_CYAN);

  // Initialize the LoRa radio
	Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

 	RadioEvents.TxDone = OnTxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxDone = OnRxDone;

  // Set LoRa Configuration
	Radio.Init( &RadioEvents );
	Radio.SetChannel( RF_FREQUENCY );
	Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
																	LORA_SPREADING_FACTOR, LORA_CODINGRATE,
																	LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
																	true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
															LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
															LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
															0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
// Initial state is to receive 
	state = STATE_RX;
}

void loop() {
	// Update our local position if it is time
	if(millis() > (posUpdate + UPDATE_POSITION)) {
		Serial.println("Updating local position");
		gps_data new_position = getGPS();
		// Keep the new GPS location if it makes sense
		if(new_position.latitude > 0.0) {
			Serial.println("Position updated");
			local_position = new_position;
		}
		posUpdate = millis();
	}

  // Know when we have lost communications with the remote
	if(data_received && (millis() > (lastComm + COMM_INTERVAL))) {
		LEDOFF();
		st7735.st7735_fill_rectangle(0, 60, 160, 20, ST7735_BLACK);
		st7735.st7735_write_str(0, 60, "Lost", Font_11x18, ST7735_RED);
		data_received = false;
	}

  // Perform functions dependent on current node state
	switch(state)
	{
		// Prepare to receive data
		case STATE_RX:
			Serial.println("Listening for packet");
			Radio.Rx(0);
			state=LOWPOWER;
			break;
		// Prepate to transmit data
		case STATE_TX:
			transmitData();
			state=LOWPOWER;
			break;
		// Process the radio interrupts until something expected happens
		case LOWPOWER:
			Radio.IrqProcess();
			break;
		// This should never run
		default:
			break;
	}
}