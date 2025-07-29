#ifndef HAND_H
#define HAND_H

#include <Arduino.h>
#include "finger.h"
#include "hand-command.h"

struct Hand {
  int finger_count;
  Finger fingers[5];

  void initialize_servos(const int min = 500, const int max = 2400, const int period = 50);
  void control_finger(enum FingersIds fingerId, int percentage);
  void control_hand(HandCommand command);
};

#endif // HAND_H