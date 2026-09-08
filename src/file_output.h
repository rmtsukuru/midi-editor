#ifndef FILE_OUTPUT_H
#define FILE_OUTPUT_H

#include "struct.h"

void load_default_midi_data(MidiFile* contents);

void save_midi_file(FILE* file, MidiFile* contents);

#endif

