#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <M5StickC.h>

#define SERVICE_UUID "766f3a63-5f57-4ae4-84f9-73c5dbc825e4"

BLEServer* bleServer = NULL;
BLEService* bleService = NULL;
bool bleEnabled = false;
bool connected = true;
int current_screen = 0;

class ServiceCallbacks: public BLEServerCallbacks {
    void onDisconnect(BLEServer* server) {
      bleEnabled = false;
    }
    void onConnect(BLEServer* server) {
      bleEnabled = true;
    };
};


void setup() {
  BLEDevice::init("HakriWatch");
  bleServer = BLEDevice::createServer();
  bleServer -> setCallbacks(new ServiceCallbacks());
  bleService = bleServer -> createService(SERVICE_UUID);
  bleService -> start();
  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv -> addServiceUUID(SERVICE_UUID);
  adv -> setScanResponse(true);
  adv -> setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  M5.begin();
}

void loop() {
  if (bleEnabled) {
    M5.Lcd.fillScreen(ORANGE);
    connected = true;
  } else if (connected) {
    bleServer -> startAdvertising();
    connected = false;
  } else {
    M5.Lcd.fillScreen(BLUE);
  }
}