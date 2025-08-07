#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "hand-command.h"


class BLECustomCallbacks : public BLECharacteristicCallbacks {
  private:
    void (*on_command_received)(HandCommand);

  public:
    BLECustomCallbacks(void (*on_command_received)(HandCommand));
    void onWrite(BLECharacteristic *characteristic) override;
};


class BLEServerCustomCallbacks : public BLEServerCallbacks {
  private:
    void (*on_bluetooth_disconnected)(HandCommand);
    BLEServer *server;
    BLEAdvertising *advertising;
    
  public:
    BLEServerCustomCallbacks(
      BLEServer *server,
      BLEAdvertising *advertising,
      void (*on_bluetooth_disconnected)(HandCommand)
    );
    
    void onConnect(BLEServer* server) override;
    void onDisconnect(BLEServer* server) override;
};


class BluetoothService {
  public:
    static void init(
      String name,
      String characteristic_uuid,
      String service_uuid,
      void (*on_command_received)(HandCommand),
      void (*on_bluetooth_disconnected)(HandCommand)
    );
};

#endif