/*
  Bluetooth controlled LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEStringCharacteristic  colorCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite, 11);

// Set the pins controlling the RGB LED
const int PIN_RED   = 3;
const int PIN_GREEN = 5;
const int PIN_BLUE  = 6;

// Function to set the RGB LED based on a string
void setLED(String rgbString) {
  int sep_1 = rgbString.indexOf(',');
  int sep_2 = rgbString.indexOf(',', (sep_1 + 1));
  int led_r = rgbString.substring(0, sep_1).toInt();
  int led_g = rgbString.substring(sep_1+1, sep_2).toInt();
  int led_b = rgbString.substring(sep_2 + 1).toInt();

  analogWrite(PIN_RED,   led_r);
  analogWrite(PIN_GREEN, led_g);
  analogWrite(PIN_BLUE,  led_b);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Arduino LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(colorCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
//  colorCharacteristic.writeValue("0,0,0");

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");

          String LEDvalue = colorCharacteristic.value();
          Serial.print("RGB Value: ");
          Serial.println(LEDvalue);
          setLED(LEDvalue);
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          setLED("0,0,0");
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
