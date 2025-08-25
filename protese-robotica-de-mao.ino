#include "bluetooth-service.h"
#include "hand.h"
#include "hand-command.h"
#include "movement-controller.h"

// Configurações do Bluetooth
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DEVICE_NAME "Bucky Barnes Arm"
#define FORCE_SENSOR_PIN 36
#define FORCE_DETECTION_LIMIT 2000

// Configurações da mão
Hand hand {
  .finger_count = 5,
  .fingers = {
    { .id = THUMB, .name= "Thumb", .min_angle = 0, .max_angle = 120, .is_inverted = false, .port = 13 },
    { .id = INDEX_FINGER, .name= "Index Finger", .min_angle = 0, .max_angle = 140, .is_inverted = false, .port = 12 },
    { .id = MIDDLE_FINGER, .name= "Middle Finger", .min_angle = 0, .max_angle = 125, .is_inverted = true, .port = 27 },
    { .id = RING_FINGER, .name= "Ring Finger", .min_angle = 0, .max_angle = 120, .is_inverted = true, .port = 25 },
    { .id = PINKY_FINGER, .name= "Pinky Finger", .min_angle = 0, .max_angle = 120, .is_inverted = true, .port = 32 }
  }
};


int readForceSensor() {
  return analogRead(FORCE_SENSOR_PIN);
}

MovementController movementController(hand, readForceSensor, FORCE_DETECTION_LIMIT);

void setup() {
  Serial.begin(115200);

  analogSetAttenuation(ADC_11db);
  hand.init();
  movementController.init();

  BluetoothService::init(
    DEVICE_NAME,
    CHARACTERISTIC_UUID,
    SERVICE_UUID,
    [](HandCommand command) { movementController.setTargetPosition(command); },
    [](HandCommand command) { movementController.setTargetPosition(command); }
  );
}

void loop() {
  movementController.update();
}