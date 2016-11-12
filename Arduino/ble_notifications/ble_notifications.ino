#include "CurieBle.h"

static const char* bluetoothDeviceName = "MyDevice"; 

static const char* serviceUuid = "1800";
static const char* characteristicUuid = "2803";
static const int   characteristicTransmissionLength = 20; 

char randomString[20] = {0};

BLEService service(serviceUuid); 
BLECharacteristic characteristic(
  characteristicUuid,
  BLERead | BLENotify, // allow client to subscribe to notifications
  characteristicTransmissionLength
);

BLEPeripheral blePeripheral; 

unsigned long bleCharacteristicLastUpdateTime_ms = 0;
unsigned long updateTimeout_ms = 5000; 

bool bleCharacteristicSubscribed = false; // true when a client subscribes

void generateRandomString(const int stringLength, char* outputString) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int alphanumLength = 36;

  outputString[stringLength];
  for (int i = 0; i < stringLength; i++) {
    outputString[i] = alphanum[random(0, alphanumLength)];
  }

  outputString[stringLength] = '\0';
}

void setBleCharacteristicValue(char* output, int length) {
  characteristic.setValue((const unsigned char*) output, length); 
}


void onBleCharacteristicSubscribed(BLECentral& central, BLECharacteristic &characteristic) {
  bleCharacteristicSubscribed = true;
  Serial.print("Characteristic ");
  Serial.print(characteristic.uuid());
  Serial.println(" subscribed to");
}

void onBleCharacteristicUnsubscribed(BLECentral& central, BLECharacteristic &characteristic) {
  bleCharacteristicSubscribed = false;
  Serial.print("Characteristic ");
  Serial.print(characteristic.uuid());
  Serial.println(" unsubscribed from");
}


void setup() {
  blePeripheral.setLocalName(bluetoothDeviceName); 

  blePeripheral.setAdvertisedServiceUuid(service.uuid()); 
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristic);


  // notify us when a client subscribes to the characteristic
  characteristic.setEventHandler(
    BLESubscribed,
    onBleCharacteristicSubscribed
  );
  characteristic.setEventHandler(
    BLEUnsubscribed,
    onBleCharacteristicUnsubscribed
  );


  blePeripheral.begin(); 
  
  randomSeed(analogRead(0)); // initialize random numbers
  
  bleCharacteristicLastUpdateTime_ms = millis();
}

void loop() {
  unsigned long currentTime_ms = millis();
  if ((currentTime_ms - bleCharacteristicLastUpdateTime_ms) > updateTimeout_ms) {
    bleCharacteristicLastUpdateTime_ms = currentTime_ms; // reset timer

    if (bleCharacteristicSubscribed) {
      int randomStringLength = random(1, characteristicTransmissionLength);
      generateRandomString(randomStringLength, randomString);
    
      setBleCharacteristicValue(randomString, randomStringLength);
    }
  }
  
}

