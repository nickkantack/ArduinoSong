
#include <Arduino.h>
#include "Note.h"
#include "MusicPlayer.h"

#define NOTE_QUEUE_SIZE 100

float _beats_per_second = 1.0;
bool _is_buzzer_high;
bool _is_playing;
bool _is_mid_song;
int _buzzer_pin = A0;
int _note_index_in_queue;
unsigned long _micros_of_last_buzzer_switch;
unsigned long _micros_of_note_start;
unsigned long _saved_micros_to_next_note;

Note _notes[NOTE_QUEUE_SIZE];
int _number_of_notes;

MusicPlayer::MusicPlayer() {
  // No configuration
}

void MusicPlayer::set_pin(int pin) {
  _buzzer_pin = pin;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void MusicPlayer::set_tempo(float beats_per_second) {
  _beats_per_second = beats_per_second;
}

void MusicPlayer::enqueue_note(Note note) {
  _notes[_number_of_notes] = note;
  _number_of_notes++;
}

void MusicPlayer::start() {
  if (_note_index_in_queue < _number_of_notes) {
    _is_playing = true;
    if (_is_mid_song) {
      _micros_of_note_start = micros() + _notes[_note_index_in_queue].get_period_micros() - _saved_micros_to_next_note;
    } else {
      _micros_of_note_start = micros();
    }
    _is_mid_song = true;
  }
}

void MusicPlayer::pause() {
  _saved_micros_to_next_note = get_micros_of_current_note_end() - (micros() - _micros_of_note_start);
  if (_is_playing) {
    _is_mid_song = true;
  }
  _is_playing = false;
}

void MusicPlayer::reset_player() {
  pause();
  _note_index_in_queue = 0;
}

void MusicPlayer::clear_queue() {
  _note_index_in_queue = 0;
  _number_of_notes = 0;
}

void MusicPlayer::handle() {
  // Flip the buzzer if needed
  unsigned long now_micros = micros();
  if (_note_index_in_queue >= _number_of_notes) {
    _is_playing = false;
    _is_mid_song = false;
    return;
  }
  Note currentNote = _notes[_note_index_in_queue];
  if (now_micros - _micros_of_last_buzzer_switch > currentNote.get_period_micros()) {
    _is_buzzer_high = !_is_buzzer_high;
    digitalWrite(_buzzer_pin, _is_buzzer_high);
    _micros_of_last_buzzer_switch = now_micros;
  }
  // Advance the note if needed
  if (now_micros - _micros_of_note_start > get_duration_of_current_note_micros()) {
    _note_index_in_queue++;
    if (_note_index_in_queue < _number_of_notes) {
      _micros_of_note_start = now_micros;
    }
  }
}

unsigned long MusicPlayer::get_duration_of_current_note_micros() {
  float seconds_per_beat = 1 / _beats_per_second;
  Note current_note = _notes[_note_index_in_queue];
  return 1000000L * seconds_per_beat * current_note.get_beat_fraction();
}

unsigned long MusicPlayer::get_micros_of_current_note_end() {
  float seconds_per_beat = 1 / _beats_per_second;
  Note current_note = _notes[_note_index_in_queue];
  unsigned long duration_of_current_note_micros = get_duration_of_current_note_micros();
  return duration_of_current_note_micros - _micros_of_note_start;
}