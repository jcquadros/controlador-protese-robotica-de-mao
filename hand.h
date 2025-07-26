#ifndef HAND_H
#define HAND_H

#include <Arduino.h>
#include "finger.h"

struct Hand {
  int finger_count;
  Finger fingers[5];
};

#endif