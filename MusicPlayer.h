
#ifndef MusicPlayer_h
#define MusicPlayer_h

#include "Note.h"

class MusicPlayer {
  public:
    MusicPlayer();
    void set_pin(int pin);
    void set_tempo(float beats_per_second);
    void enqueue_note(Note note);
    void start();
    void pause();
    void reset_player();
    void clear_queue();
    void handle();
  private:
    unsigned long get_duration_of_current_note_micros();
    unsigned long get_micros_of_current_note_end();
};

#endif