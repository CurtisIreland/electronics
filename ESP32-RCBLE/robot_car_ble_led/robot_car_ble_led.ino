/*
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BLE2901.h"

BLEServer *pServer = NULL;
BLECharacteristic * pCharacteristicA1;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// Create Service A - ffe0
#define SERVICE_A_UUID           "0000FFE0-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_A1_UUID   "0000FFE1-0000-1000-8000-00805F9B34FB"

int LED=2; // Define pin for on-board LED
volatile int state = LOW; // The initial state of the LED pin

void stateChange(){
  state = !state;
  digitalWrite(LED, state);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("BLE Connected");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("BLE Disconnected");
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        if(rxValue[0] == 'A') {
          stateChange();
        }

        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
        Serial.println();
      }
    }
};


void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ELEGOO Car");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pServiceA = pServer->createService(SERVICE_A_UUID);

  // Create a BLE Characteristic
  pCharacteristicA1 = pServiceA->createCharacteristic(
										CHARACTERISTIC_A1_UUID,
										BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR
									);
                      
  pCharacteristicA1->addDescriptor(new BLE2902());
  pCharacteristicA1->addDescriptor(new BLE2901());
  pCharacteristicA1->setCallbacks(new MyCallbacks());

  // Start the service
  pServiceA->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    if (deviceConnected) {
//        pCharacteristicA1->notify();
		    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	  }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
