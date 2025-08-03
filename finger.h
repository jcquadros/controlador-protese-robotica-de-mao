#ifndef FINGER_H
#define FINGER_H

#include <Arduino.h>
#include <ESP32Servo.h>

enum FingersIds {
  THUMB = 0,
  INDEX_FINGER = 1,
  MIDDLE_FINGER = 2,
  RING_FINGER = 3,
  PINKY_FINGER = 4
};

struct Finger {
  int id;
  String name;
  int min_angle;
  int max_angle;
  bool is_inverted;
  int port;
  Servo servo;

  void initialize_servo(const int min = 500, const int max = 2400, const int period = 50);
  int map_percentage_to_angle(int percentage) const;
  void send_percentage(const int percentage);
  void send_angle(int angle, bool log = true);
};

#endif