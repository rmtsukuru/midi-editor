#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "struct.h"

int load_variable_length_value(FILE* file, int* result);

void load_header_chunk(FILE* file, Header* header);

int load_meta_event(FILE* file, TrackEvent* event);

int load_track_event(FILE* file, TrackEvent* event, byte previous);

void load_track_chunk(FILE* file, Track* track);

void load_midi_file(FILE* file, MidiFile* contents);

#endif

