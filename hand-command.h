#ifndef HAND_COMMAND_H
#define HAND_COMMAND_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct HandCommand {
  int thumb;
  int index;
  int middle;
  int ring;
  int pinky;

  HandCommand(int t = 0, int i = 0, int m = 0, int r = 0, int p = 0);

  static HandCommand fromJson(const String& jsonString);
  String toJson() const;
  bool equals(const HandCommand& other) const;
};

#endif