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

// Pin definitions to control ELEGOO car
int LED=2; // Define pin for on-board LED
volatile int state = LOW; // The initial state of the LED pin
int in1=17;
int in2=16;
int in3=4;
int in4=15;
int ENA=5;
int ENB=27;
int ABS=150;

// Functions to manage motor control
void _mForward()
{ 
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,HIGH);//digital output
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("Forward");
}
void _mBack()
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("Back");
}
void _mleft()
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,LOW);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW); 
  Serial.println("go left!");
}
void _mright()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("go right!");
}
void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("Stop!");
}

// Change LED State
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
        if(rxValue[0]=='f')
          {
            _mForward();
          }
          else if(rxValue[0]=='b')
          {
            _mBack();
            delay(200);
          }
          else if(rxValue[0]=='l')
          {
            _mleft();
            delay(200);
          }
          else if(rxValue[0]=='r')
          {
            _mright();
            delay(200);
          }
          else if(rxValue[0]=='s')
          {
             _mStop();     
          }
          else if(rxValue[0]=='A')
          {
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
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  _mStop();

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
