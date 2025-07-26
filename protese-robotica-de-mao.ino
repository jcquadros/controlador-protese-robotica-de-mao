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

#define FINGER_COUNT 5

#define THUMB_PORT 32
#define INDEX_FINGER_PORT 33
#define MIDDLE_FINGER_PORT 25
#define RING_FINGER_PORT 26
#define PINKY_FINGER_PORT 27

enum FingersEnum {
  THUMB = 0,
  INDEX_FINGER = 1,
  MIDDLE_FINGER = 2,
  RING_FINGER = 3,
  PINKY_FINGER = 4
};

const Hand hand {
  FINGER_COUNT,
  {
    {THUMB, 0, 180},
    {INDEX_FINGER, 0, 180},
    {MIDDLE_FINGER, 0, 180},
    {RING_FINGER, 0, 180},
    {PINKY_FINGER, 0, 180}
  }
};


Servo servo_thumb;
Servo servo_index_finger;
Servo servo_middle_finger;
Servo servo_ring_finger;
Servo servo_pinky_finger;


void control_finger(enum FingersEnum finger, int percentage) {
  const Finger& fingerConfig = hand.fingers[finger];
  const int angle = fingerConfig.mapPercentageToAngle(percentage);

  switch(finger) {
    case THUMB:
      Serial.print("Writing to thumb value: ");
      Serial.println(angle);
      servo_thumb.write(angle);
      break;
    case INDEX_FINGER:
      Serial.print("Writing to index finger value: ");
      Serial.println(angle);
      servo_index_finger.write(angle);
      break;
    case MIDDLE_FINGER:
      Serial.print("Writing to middle finger value: ");
      Serial.println(angle);
      servo_middle_finger.write(angle);
      break;
    case RING_FINGER:
      Serial.print("Writing to ring finger value: ");
      Serial.println(angle);
      servo_ring_finger.write(angle);
      break;
    case PINKY_FINGER:
      Serial.print("Writing to pinky finger value: ");
      Serial.println(angle);
      servo_pinky_finger.write(angle);
      break;
  }
}

void control_hand(HandCommand command) {
  control_finger(THUMB, command.thumb);
  control_finger(INDEX_FINGER, command.index);
  control_finger(MIDDLE_FINGER, command.middle);
  control_finger(RING_FINGER, command.ring);
  control_finger(PINKY_FINGER, command.pinky);
}


class BLECustomCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    auto value = characteristic->getValue();
    Serial.print("Received value: ");
    Serial.println(value.c_str());

    auto command = HandCommand::fromJson(value);
    control_hand(command);
  }
};


void setup() {
  Serial.begin(115200);
  
  // Alocar timer disponível (0 a 3)
  // ESP32PWM::allocateTimer(0);

  // Configurar frequência de operação dos servos
  servo_thumb.setPeriodHertz(50);
  servo_index_finger.setPeriodHertz(50);
  servo_middle_finger.setPeriodHertz(50);
  servo_ring_finger.setPeriodHertz(50);
  servo_pinky_finger.setPeriodHertz(50);

  // Associar cada servo a um pino e definir pulso mínimo e máximo (em microssegundos)
  // 0° = 500us, 180° = 2400us
  servo_thumb.attach(THUMB_PORT, 500, 2400);
  servo_index_finger.attach(INDEX_FINGER_PORT, 500, 2400);
  servo_middle_finger.attach(MIDDLE_FINGER_PORT, 500, 2400);
  servo_ring_finger.attach(RING_FINGER_PORT, 500, 2400);
  servo_pinky_finger.attach(PINKY_FINGER_PORT, 500, 2400);

  analogReadResolution(12);

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
  int adcValue = analogRead(POT_PIN); // Lê valor ADC (0–4095)
  float voltage = (adcValue/ADC_MAX) * VMAX; // Converte para volts
  
  // Serial.print("ADC: ");
  // Serial.print(adcValue);
  // Serial.print(" | Tensão: ");
  // Serial.print(voltage, 3); // Mostra 3 casas decimais
  // Serial.println(" V");

  float angle = (adcValue/ADC_MAX) * ANGLE_MAX; // Converte para angulos

  if(angle >= (ANGLE_MAX-ANGLE_ERRO))
    angle = ANGLE_MAX;

  if(angle <= (ANGLE_MIN+ANGLE_ERRO))
    angle = ANGLE_MIN;

  // Serial.print("ANGLE: ");
  // Serial.println(angle);
  // Serial.println();

  servo_thumb.write(angle);
  delay(500);
}