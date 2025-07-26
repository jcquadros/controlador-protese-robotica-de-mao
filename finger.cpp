#include "finger.h"

void Finger::initialize_servo(const int min, const int max, const int period) {
  // Configurar frequência de operação dos servos
  servo.setPeriodHertz(period);
  servo.attach(port, min, max);
}

int Finger::map_percentage_to_angle(int percentage) const {
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;

  return (int)(min_angle + (max_angle - min_angle) * (percentage / 100.0f));
}

void Finger::send_percentage(const int percentage) {
  int angle = map_percentage_to_angle(percentage);
  send_angle(angle);
}

void Finger::send_angle(int angle) {
  servo.write(angle);

  Serial.print("Writing to finger (");
  Serial.print(name);
  Serial.print(") value: ");
  Serial.println(angle);
}