#include "hand.h"

void Hand::init(const int min, const int max, const int period) {
  for (int i = 0; i < finger_count; i++) {
    Finger& finger = fingers[i];
    finger.initialize_servo(min, max, period);
  }

  // Open hand on start
  control_hand(HandCommand());
}

void Hand::control_finger(enum FingersIds fingerId, int percentage) {
  Finger& finger = fingers[fingerId];
  finger.send_percentage(percentage);
}

void Hand::control_hand(HandCommand command) {
  // Atualiza o estado interno antes de mover os dedos
  currentCommand = command;

  control_finger(THUMB, command.thumb);
  control_finger(INDEX_FINGER, command.index);
  control_finger(MIDDLE_FINGER, command.middle);
  control_finger(RING_FINGER, command.ring);
  control_finger(PINKY_FINGER, command.pinky);
}