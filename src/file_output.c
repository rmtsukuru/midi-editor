#include <stdio.h>

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

// Return number of bytes written
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

// Return number of bytes written
int save_midi_file(FILE* file, MidiFile* contents) {
    int bytes = write_header_chunk(file, &contents->header);

    for (int i = 0; i < contents->header.track_count; i++) {
        bytes += write_track_chunk(file, &contents->tracks[i]);
    }
    return bytes;
}

