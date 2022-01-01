
#ifndef Note_h
#define Note_h

class Note {
  public:
    Note();
    Note(float beat_fraction, float frequency);
    float get_beat_fraction();
    float get_frequency();
    unsigned long get_period_micros();
  private:
    float _frequency;
    float _beat_fraction;
    unsigned long _period_micros;
};

#endif