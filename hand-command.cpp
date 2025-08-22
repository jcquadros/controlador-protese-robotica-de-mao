#include "hand-command.h"

HandCommand::HandCommand(int t, int i, int m, int r, int p)
    : thumb(t), index(i), middle(m), ring(r), pinky(p) {}

HandCommand HandCommand::fromJson(const String& jsonString) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return HandCommand(); // default
  }

  return HandCommand(
    doc["thumb"] | 0,
    doc["index"] | 0,
    doc["middle"] | 0,
    doc["ring"] | 0,
    doc["pinky"] | 0
  );
}

String HandCommand::toJson() const {
  StaticJsonDocument<200> doc;
  doc["thumb"] = thumb;
  doc["index"] = index;
  doc["middle"] = middle;
  doc["ring"] = ring;
  doc["pinky"] = pinky;

  String output;
  serializeJson(doc, output);
  return output;
}

bool HandCommand::equals(const HandCommand& other) const {
  return thumb == other.thumb &&
          index == other.index &&
          middle == other.middle &&
          ring == other.ring &&
          pinky == other.pinky;
}