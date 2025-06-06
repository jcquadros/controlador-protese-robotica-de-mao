#include <ESP32Servo.h>

#define THUMB_PORT 32
#define INDEX_FINGER_PORT 33
#define MIDDLE_FINGER_PORT 25
#define RING_FINGER_PORT 26
#define PINKY_FINGER_PORT 27

// Definindo os pinos
#define POT_PIN 25

#define FINGER_COUNT 5

enum Finger {
  THUMB = 32,
  INDEX_FINGER = 33,
  MIDDLE_FINGER = 25,
  RING_FINGER = 26,
  PINKY_FINGER = 27
};

enum Command {
  CLOSE_HAND,
  OPEN_HAND
};

const float POT = 10;
const float R = 1;
const float VREF = 3.3;
const float ADC_RESOLUTION = 4095;

const float VMAX = (VREF * (POT/(POT+R)));
const float ADC_MAX = (VMAX/VREF)*ADC_RESOLUTION;

const float ANGLE_MIN = 0;
const float ANGLE_MAX = 180;
const float ANGLE_ERRO = 10;

Finger ordered_fingers[] = { THUMB, INDEX_FINGER, MIDDLE_FINGER, RING_FINGER, PINKY_FINGER };

float open_hand_command[] = { ANGLE_MIN, ANGLE_MIN, ANGLE_MIN, ANGLE_MIN, ANGLE_MIN };
float close_hand_command[] = { ANGLE_MAX, ANGLE_MAX, ANGLE_MAX, ANGLE_MAX, ANGLE_MAX };

#define INDEX 0

Servo servo_thumb;
Servo servo_index_finger;
Servo servo_middle_finger;
Servo servo_ring_finger;
Servo servo_pinky_finger;

float* command_to_finger_angle_mapper(enum Command command) {
  switch(command) {
    case CLOSE_HAND:
      return close_hand_command;
    case OPEN_HAND:
    default:
      return open_hand_command;
  }
}

void control_finger(enum Finger finger, int angle) {
  switch(finger) {
    case THUMB:
      servo_thumb.write(angle);
      break;
    case INDEX_FINGER:
      servo_index_finger.write(angle);
      break;
    case MIDDLE_FINGER:
      servo_middle_finger.write(angle);
      break;
    case RING_FINGER:
      servo_ring_finger.write(angle);
      break;
    case PINKY_FINGER:
      servo_pinky_finger.write(angle);
      break;
  }
}

void control_hand(enum Command command) {
  float* angles = command_to_finger_angle_mapper(command);

  for(int i = 0; i < FINGER_COUNT; i++) {
    control_finger(ordered_fingers[i], angles[i]);
  }
}

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
  servo_thumb.attach(THUMB_PORT, 500, 2400); // 0° = 500us, 180° = 2400us
  servo_index_finger.attach(INDEX_FINGER_PORT, 500, 2400); // 0° = 500us, 180° = 2400us
  servo_middle_finger.attach(MIDDLE_FINGER_PORT, 500, 2400); // 0° = 500us, 180° = 2400us
  servo_ring_finger.attach(RING_FINGER_PORT, 500, 2400); // 0° = 500us, 180° = 2400us
  servo_pinky_finger.attach(PINKY_FINGER_PORT, 500, 2400); // 0° = 500us, 180° = 2400us

  analogReadResolution(12);
}

void loop() {
  int adcValue = analogRead(POT_PIN); // Lê valor ADC (0–4095)
  float voltage = (adcValue/ADC_MAX) * VMAX; // Converte para volts
  
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Tensão: ");
  Serial.print(voltage, 3); // Mostra 3 casas decimais
  Serial.println(" V");

  float angle = (adcValue/ADC_MAX) * ANGLE_MAX; // Converte para angulos

  if(angle >= (ANGLE_MAX-ANGLE_ERRO))
    angle = ANGLE_MAX;

  if(angle <= (ANGLE_MIN+ANGLE_ERRO))
    angle = ANGLE_MIN;

  Serial.print("ANGLE: ");
  Serial.println(angle);
  Serial.println();

  servo_thumb.write(angle);
  delay(500);
}
