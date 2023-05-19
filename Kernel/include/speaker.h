#ifndef SPEAKER_H
#define SPEAKER_H

extern void _beep_start(uint64_t real_frequency);

extern void _beep_stop();

void beep(uint64_t n_frequency);

void stop_beep();

#endif