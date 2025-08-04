#include "bluetooth-service.h"

BLECustomCallbacks::BLECustomCallbacks(void (*on_command_received)(HandCommand))
  : on_command_received(on_command_received) {}

void BLECustomCallbacks::onWrite(BLECharacteristic *characteristic) {
  auto value = characteristic->getValue();

  Serial.print("Received command: ");
  Serial.println(value.c_str());

  auto command = HandCommand::fromJson(value);
  on_command_received(command);
}


BLEServerCustomCallbacks::BLEServerCustomCallbacks(BLEServer *server, BLEAdvertising *advertising) 
  : server(server), advertising(advertising) {}

void BLEServerCustomCallbacks::onConnect(BLEServer* server) {
  Serial.println("Device connected");
}

void BLEServerCustomCallbacks::onDisconnect(BLEServer* server) {
  Serial.println("Device disconnected - restarting advertising");
  // Restart advertising when device disconnects
  advertising->start();
  Serial.println("Advertising restarted");
}


void BluetoothService::init(String name, String characteristic_uuid, String service_uuid, void (*on_command_received)(HandCommand)) {
  Serial.println("Starting BLE server!");
  
  BLEDevice::init(name);
  BLEServer *server = BLEDevice::createServer();
  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  server->setCallbacks(new BLEServerCustomCallbacks(server, advertising));

  BLEService *service = server->createService(service_uuid);
  BLECharacteristic *characteristic = service->createCharacteristic(
    characteristic_uuid,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  characteristic->setCallbacks(new BLECustomCallbacks(on_command_received));
  service->start();

  advertising->addServiceUUID(service_uuid);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Bluetooth server started!");
}