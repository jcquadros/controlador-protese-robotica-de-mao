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

class BluetoothService {
  public:
    static void init(String name, String characteristic_uuid, String service_uuid, void (*on_command_received)(HandCommand));
};

#endif