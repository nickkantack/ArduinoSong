
#include "Note.h"
#include "MusicPlayer.h"

MusicPlayer mp = MusicPlayer();

bool is_reset;
long millis_to_reset = millis() + 3000;

void setup() {
  mp.set_pin(4);
  mp.enqueue_note(Note(0.1, 1000));
  mp.enqueue_note(Note(0.1, 2000));
  mp.enqueue_note(Note(0.1, 1508));
  mp.start();
}

void loop() {
  mp.handle();

  // At a predetermined time, reset the player
  if (!is_reset && millis() > millis_to_reset) {
    mp.reset_player();
    mp.start();
    is_reset = true;
  }
  
}
