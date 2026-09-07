#ifndef OUTPUT_H
#define OUTPUT_H

#include "struct.h"

void print_header_chunk(Header* header);

void print_track_chunk(Track* track, byte track_number);

void print_hex_data(FILE* file);


#endif

