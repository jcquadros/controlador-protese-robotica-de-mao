#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "hand-command.h"
#include "hand.h"

// Definindo os pinos
#define POT_PIN 25

const float POT = 10;
const float R = 1;
const float VREF = 3.3;
const float ADC_RESOLUTION = 4095;

const float VMAX = (VREF * (POT/(POT+R)));
const float ADC_MAX = (VMAX/VREF)*ADC_RESOLUTION;

const float ANGLE_MIN = 0;
const float ANGLE_MAX = 130;
const float ANGLE_ERRO = 10;


// Configurações do Bluetooth
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// Configurações da mão
Hand hand {
  .finger_count = 5,
  .fingers = {
    { .id = THUMB, .name= "Thumb", .min_angle = 0, .max_angle = 130, .is_inverted = false, .port = 13 },
    { .id = INDEX_FINGER, .name= "Index Finger", .min_angle = 0, .max_angle = 150, .is_inverted = false, .port = 12 },
    { .id = MIDDLE_FINGER, .name= "Middle Finger", .min_angle = 0, .max_angle = 120, .is_inverted = true, .port = 27 },
    { .id = RING_FINGER, .name= "Ring Finger", .min_angle = 0, .max_angle = 120, .is_inverted = true, .port = 25 },
    { .id = PINKY_FINGER, .name= "Pinky Finger", .min_angle = 0, .max_angle = 100, .is_inverted = true, .port = 32 }
  }
};


class BLECustomCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    auto value = characteristic->getValue();

    Serial.print("Received command: ");
    Serial.println(value.c_str());

    auto command = HandCommand::fromJson(value);
    hand.control_hand(command);
  }
};


void setup() {
  Serial.begin(115200);
  
  // Alocar timer disponível (0 a 3)
  // ESP32PWM::allocateTimer(0);
  analogReadResolution(12);

  hand.initialize_servos();

  // Configurações do servidor Bluetooth
  Serial.println("Starting BLE work!");
  BLEDevice::init("Bucky Barnes Arm");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new BLECustomCallbacks());
  // pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // int adcValue = analogRead(POT_PIN); // Lê valor ADC (0–4095)
  // float voltage = (adcValue/ADC_MAX) * VMAX; // Converte para volts
  
  // Serial.print("ADC: ");
  // Serial.print(adcValue);
  // Serial.print(" | Tensão: ");
  // Serial.print(voltage, 3); // Mostra 3 casas decimais
  // Serial.println(" V");

  // float angle = (adcValue/ADC_MAX) * ANGLE_MAX; // Converte para angulos

  // if(angle >= (ANGLE_MAX-ANGLE_ERRO))
  //   angle = ANGLE_MAX;

  // if(angle <= (ANGLE_MIN+ANGLE_ERRO))
  //   angle = ANGLE_MIN;

  // Serial.print("ANGLE: ");
  // Serial.println(angle);
  // Serial.println();

  // Finger& finger = (Finger&) hand.fingers[THUMB];
  // finger.sendAngle(angle);
  // delay(500);
}