#include <stdio.h>
#include <string.h>

#include "util.h"
#include "file_output.h"

// Return number of bytes written
int write_variable_length_value(FILE* file, int value) {
    int buffer = value & 0x7f;
    int byte_count = 0;

    while ((value >>= 7) > 0) {
        buffer <<= 8;
        buffer |= 0x80;
        buffer += (value & 0x7f);
    }

    byte done = 0;
    while (!done) {
        putc(buffer, file);
        byte_count++;
        if (buffer & 0x80) {
            buffer >>= 8;
        } else {
            done = 1;
        }
    }
    return byte_count;
}

byte is_two_value_event_type(EventType type) {
    return type == NOTE_ON || type == NOTE_OFF || type == NOTE_AFTERTOUCH ||
                type == CONTROLLER_EVENT || type == PITCH_BEND_EVENT;
}

// TODO move this to a different file maybe?
void load_default_midi_data(MidiFile* contents) {
    Header* header = &contents->header;
    strcpy(header->chunk_type, "MThd");
    header->length = 6;
    header->format = 1;
    header->track_count = 1;
    header->division = 96;

    Track* track = &contents->tracks[0];
    strcpy(track->chunk_type, "MTrk");

    // Track events
    TrackEvent* events = track->events;
    events[0].delta_time = 0;               // 3 bytes
    events[0].type = PROGRAM_CHANGE;
    events[0].type_code = 0xC0;
    events[0].value1 = 0; // Grand Piano

    events[1].delta_time = 0;               // 4 bytes
    events[1].type = NOTE_ON;
    events[1].type_code = 0x90;
    events[1].value1 = 71; // B4
    events[1].value2 = 64;

    events[2].delta_time = 96;              // 4 bytes
    events[2].type = NOTE_ON;
    events[2].type_code = 0x90;
    events[2].value1 = 69; // A4
    events[2].value2 = 64;

    events[3].delta_time = 0;               // 4 bytes
    events[3].type = NOTE_OFF;
    events[3].type_code = 0x80;
    events[3].value1 = 71; // B4
    events[3].value2 = 64;

    events[4].delta_time = 96;              // 4 bytes
    events[4].type = NOTE_ON;
    events[4].type_code = 0x90;
    events[4].value1 = 65; // F4
    events[4].value2 = 64;

    events[5].delta_time = 0;               // 4 bytes
    events[5].type = NOTE_OFF;
    events[5].type_code = 0x80;
    events[5].value1 = 69; // A4
    events[5].value2 = 64;

    events[6].delta_time = 96;              // 4 bytes
    events[6].type = NOTE_OFF;
    events[6].type_code = 0x80;
    events[6].value1 = 65; // F4
    events[6].value2 = 64;

    events[7].delta_time = 0;               // 4 bytes
    events[7].type = META_EVENT;
    events[7].type_code = 0xFF;
    events[7].meta_type = END_OF_TRACK;
    events[7].meta_type_code = 0x2F;
    events[7].meta_length = 0;

    track->events_length = 8;
    track->length = 31;                     // 31 bytes total
}

// Return number of bytes written
// TODO double check this is all correct
// (do I need a save_bytes function???)
int write_header_chunk(FILE* file, Header* header) {
    byte buffer[4];
    fwrite("MThd", 4, 1, file);
    write_bytes(buffer, 6, 4);
    fwrite(buffer, 4, 1, file);
    write_bytes(buffer, header->format, 2);
    fwrite(buffer, 2, 1, file);
    write_bytes(buffer, header->track_count, 2);
    fwrite(buffer, 2, 1, file);
    write_bytes(buffer, header->division, 2);
    fwrite(buffer, 2, 1, file);
    return 14; // 8 + 6
}

// Return number of bytes written
int write_meta_event(FILE* file, TrackEvent* event) {
    int byte_count = 1;
    // TODO write a function to assign the meta type code based on meta type
    putc(event->meta_type_code, file);
    byte_count += write_variable_length_value(file, event->meta_length);

    // TODO handle other meta events besides end of track

    return byte_count;
}

// Return number of bytes written
int write_track_event(FILE* file, TrackEvent* event, byte previous) {
    // Write delta time
    int byte_count = write_variable_length_value(file, event->delta_time);

    // Then write event data (starting with event type)
    putc(event->type_code, file);
    // TODO write a function to assign the type code based on type 
    // (to simplify data entry)
    byte_count++;
    if (event->type == SYSEX_EVENT) {
        // TODO handle these pls
    } else if (event->type == META_EVENT) {
        byte_count += write_meta_event(file, event);
    } else if (is_two_value_event_type(event->type)) {
        // TODO Maybe handle previous?
        putc(event->value1, file);
        putc(event->value2, file);
        byte_count += 2;
    } else if (event->type == PROGRAM_CHANGE || event->type == CHANNEL_AFTERTOUCH) {
        putc(event->value1, file);
        byte_count++;
    }
    return byte_count;
}

// Return number of bytes written
int write_track_chunk(FILE* file, Track* track) {
    byte buffer[4];
    fwrite("MTrk", 4, 1, file);
    write_bytes(buffer, track->length, 4);
    fwrite(buffer, 4, 1, file);
    int byte_count = 0;
    byte previous = 0x00;
    for (int i = 0; i < track->events_length; i++) {
        byte_count += write_track_event(file, &track->events[i], previous);
        previous = track->events[i].type_code;
    }
    return byte_count;
}

void save_midi_file(FILE* file, MidiFile* contents) {
    write_header_chunk(file, &contents->header);

    for (int i = 0; i < contents->header.track_count; i++) {
        write_track_chunk(file, &contents->tracks[i]);
    }
}

