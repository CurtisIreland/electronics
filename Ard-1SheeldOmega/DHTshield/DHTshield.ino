
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_SHIELD

/* Include 1Sheeld library. */ 
#include <OnionLibrary.h>
#include <OneSheeld.h>
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);
Onion* onionSetup;

void setup() {
  onionSetup = new Onion;
  /* Start communication. */
  OneSheeld.begin();
 
  dht.begin();
  Terminal.println("DHTxx test!");
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Terminal.println("Failed to read from DHT");
  } else {
    Terminal.print("Humidity: "); 
    Terminal.print(h);
    Terminal.print(" %\t");
    Terminal.print("Temperature: "); 
    Terminal.print(t);
    Terminal.println(" *C");
  }
}
