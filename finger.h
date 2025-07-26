#ifndef FINGER_H
#define FINGER_H

#include <Arduino.h>

struct Finger {
  int id;
  int max_angle;
  int min_angle;

  int mapPercentageToAngle(int percentage) const {
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    return (int) (min_angle + (max_angle - min_angle) * (percentage / 100.0f)); 
  }
};

#endif