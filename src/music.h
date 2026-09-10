#ifndef MUSIC_H
#define MUSIC_H

#include "struct.h"

char* get_note_name(byte note_number);

char* get_instrument_name(byte program_number);

void load_default_midi_data(MidiFile* contents);

#endif

