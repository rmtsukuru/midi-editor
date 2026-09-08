#include <stdio.h>
#include <string.h>

#include "file_output.h"

// TODO move this to a different file maybe?
void load_default_midi_data(MidiFile* contents) {
    Header* header = &contents->header;
    strcpy(header->chunk_type, "MThd");
    header->length = 6;
    header->format = 1;
    header->track_count = 1;
    header->division = 500;

    Track* track = &contents->tracks[0];
    strcpy(track->chunk_type, "MTrk");

    // Track events
    TrackEvent* events = track->events;
    events[0].delta_time = 0;               // 3 bytes
    events[0].type = PROGRAM_CHANGE;
    events[0].value1 = 0; // Grand Piano

    events[1].delta_time = 0;               // 4 bytes
    events[1].type = NOTE_ON;
    events[1].value1 = 71; // B4
    events[1].value2 = 64;

    events[2].delta_time = 200;             // 5 bytes
    events[2].type = NOTE_ON;
    events[2].value1 = 69; // A4
    events[2].value2 = 64;

    events[3].delta_time = 0;               // 4 bytes
    events[3].type = NOTE_OFF;
    events[3].value1 = 71; // B4
    events[3].value2 = 64;

    events[4].delta_time = 200;             // 5 bytes
    events[4].type = NOTE_ON;
    events[4].value1 = 65; // F4
    events[4].value2 = 64;

    events[5].delta_time = 0;               // 4 bytes
    events[5].type = NOTE_OFF;
    events[5].value1 = 69; // A4
    events[5].value2 = 64;

    events[6].delta_time = 200;             // 5 bytes
    events[6].type = NOTE_OFF;
    events[6].value1 = 65; // F4
    events[6].value2 = 64;

    // x is the last event
    events[7].delta_time = 0;               // 4 bytes
    events[7].type = META_EVENT;
    events[7].meta_type = END_OF_TRACK;
    events[7].meta_length = 0;

    track->events_length = 8;
    track->length = 34;                     // 34 bytes total
}

// Return number of bytes written
int write_header_chunk(FILE* file, Header* header) {
    // TODO finish this pls
}

// Return number of bytes written
int write_track_chunk(FILE* file, Track* track) {
    // TODO finish this too
}

void save_midi_file(FILE* file, MidiFile* contents) {
    write_header_chunk(file, &contents->header);

    for (int i = 0; i < contents->header.track_count; i++) {
        write_track_chunk(file, &contents->tracks[i]);
    }
}

