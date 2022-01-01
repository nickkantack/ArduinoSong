
#include <Arduino.h>
#include "Note.h"

Note::Note() {
  
}

Note::Note(float beat_fraction, float frequency) {
  _beat_fraction = beat_fraction;
  _frequency = frequency;
  _period_micros = 1000000L / _frequency;
}

float Note::get_beat_fraction() {
  return _beat_fraction;
}

float Note::get_frequency() {
  return _frequency;
}

unsigned long Note::get_period_micros() {
  return _period_micros;
}