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

void BluetoothService::init(String name, String characteristic_uuid, String service_uuid, void (*on_command_received)(HandCommand)) {
  Serial.println("Starting BLE work!");
  BLEDevice::init(name);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(service_uuid);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    characteristic_uuid,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new BLECustomCallbacks(on_command_received));
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(service_uuid);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}