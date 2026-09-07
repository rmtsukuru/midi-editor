#include <stdio.h>

#include "util.h"
#include "file_input.h"

int load_variable_length_value(FILE* file, int* result) {
    int bytes_read = 1;
    int value = getc(file);
    byte ch;
    if (value & 0x80) {
        value &= 0x7f;
        do {
            ch = getc(file);
            bytes_read++;
            value = (value << 7) + (ch & 0x7f);
        } while (ch & 0x80);
    }

    *result = value;
    return bytes_read;
}

void load_header_chunk(FILE* file, Header* header) {
    byte buffer[8];
    fread(header->chunk_type, 4, 1, file);
    header->chunk_type[4] = 0;
    fread(buffer, 4, 1, file);
    header->length = load_bytes(buffer, 4);
    fread(buffer, 2, 1, file);
    header->format = load_bytes(buffer, 2);
    fread(buffer, 2, 1, file);
    header->track_count = load_bytes(buffer, 2);
    fread(buffer, 2, 1, file);
    header->division = load_bytes(buffer, 2);
    // TODO add validation to confirm that chunk_type and length are correct
}

int load_meta_event(FILE* file, TrackEvent* event) {
    int bytes_read = 1;
    char ch = getc(file);
    event->meta_type_code = ch;
    int length;
    switch (ch) {
        case 0x00:
            event->meta_type = SEQUENCE_NUMBER;
            // TODO implement this
            break;
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            if (ch == 0x01) {
                event->meta_type = TEXT_EVENT;
            } else if (ch == 0x02) {
                event->meta_type = COPYRIGHT;
            } else if (ch == 0x03) {
                event->meta_type = TRACK_NAME;
            } else if (ch == 0x04) {
                event->meta_type = INSTRUMENT_NAME;
            } else if (ch == 0x05) {
                event->meta_type = LYRIC;
            } else if (ch == 0x06) {
                event->meta_type = MARKER;
            } else if (ch == 0x07) {
                event->meta_type = CUE_POINT;
            }
            bytes_read += load_variable_length_value(file, &length);
            fread(event->metadata, length, 1, file); // Text data
            bytes_read += length;
            event->meta_length = length;
            break;
        case 0x20:
            event->meta_type = CHANNEL_PREFIX;
            event->meta_length = getc(file); // Should always equal 1.
            event->metadata[0] = getc(file); // MIDI channel
            bytes_read += 2;
            break;
        case 0x2F:
            event->meta_type = END_OF_TRACK;
            event->meta_length = getc(file); // Should always equal 0.
            bytes_read++;
            break;
        case 0x51:
            event->meta_type = SET_TEMPO;
            event->meta_length = getc(file); // Should always equal 3.
            bytes_read++;
            // Tempo in microseconds per quarter note (24-bit number).
            fread(event->metadata, 3, 1, file);
            bytes_read += 3;
            break;
        case 0x54:
            event->meta_type = SMPTE_OFFSET;
            // TODO implement this later
            break;
        case 0x58:
            event->meta_type = TIME_SIGNATURE;
            event->meta_length = getc(file); // Should always equal 4.
            bytes_read++;
            // 4 bytes used to specify a time signature
            // Numerator
            // Denominator (negative power of 2, so 2 is a quarter note)
            // Number of MIDI clocks in a metronome click (typically 24 or 0x18
            // Number of 32nd notes per quarter note (24 MIDI clocks), typically 8
            fread(event->metadata, 4, 1, file);
            bytes_read += 4;
            break;
        case 0x59:
            event->meta_type = KEY_SIGNATURE;
            // TODO implement this later
            break;
        case 0x7F:
            event->meta_type = SEQUENCER_SPECIFIC;
            bytes_read += load_variable_length_value(file, &length);
            fread(event->metadata, length, 1, file); // Sequencer data
            bytes_read += length;
            event->meta_length = length;
    }

    return bytes_read;
}

int load_track_event(FILE* file, TrackEvent* event, byte previous) {
    // First load delta time
    int bytes_read = load_variable_length_value(file, &event->delta_time);

    // Then load event data (starting with event type)
    byte ch = getc(file);
    byte value1 = 0x80;
    bytes_read++;
    if (ch < 0x80 && previous != 0x00) {
        value1 = ch;
        ch = previous;
    }
    event->type_code = ch;
    if (ch == 0xF0 || ch == 0xF7) {
        event->type = SYSEX_EVENT;
        // TODO parse these...
    } else if (ch == 0xFF) {
        event->type = META_EVENT;
        bytes_read += load_meta_event(file, event);
    } else if ((ch & 0xF0) == 0x80) {
        event->type = NOTE_OFF;
        if (value1 < 0x80) {
            event->value1 = value1;
            bytes_read--;
        } else {
            event->value1 = getc(file); // Note number
        }
        event->value2 = getc(file);     // Velocity
        bytes_read += 2;
    } else if ((ch & 0xF0) == 0x90) {
        event->type = NOTE_ON;
        if (value1 < 0x80) {
            event->value1 = value1;
            bytes_read--;
        } else {
            event->value1 = getc(file); // Note number
        }
        event->value2 = getc(file);     // Velocity
        bytes_read += 2;
    } else if ((ch & 0xF0) == 0xA0) {
        event->type = NOTE_AFTERTOUCH;
        if (value1 < 0x80) {
            event->value1 = value1;
            bytes_read--;
        } else {
            event->value1 = getc(file); // Note number
        }
        event->value2 = getc(file);     // Pressure amount
        bytes_read += 2;
    } else if ((ch & 0xF0) == 0xB0) {
        event->type = CONTROLLER_EVENT;
        if (value1 < 0x80) {
            event->value1 = value1;
            bytes_read--;
        } else {
            event->value1 = getc(file); // Controller number
        }
        event->value2 = getc(file);     // Value
        bytes_read += 2;
    } else if ((ch & 0xF0) == 0xC0) {
        event->type = PROGRAM_CHANGE;
        if (value1 < 0x80) {
            event->value1 = value1;
        } else {
            event->value1 = getc(file); // Program (instrument) number
            bytes_read += 1;
        }
    } else if ((ch & 0xF0) == 0xD0) {
        event->type = CHANNEL_AFTERTOUCH;
        if (value1 < 0x80) {
            event->value1 = value1;
        } else {
            event->value1 = getc(file); // Pressure amount
            bytes_read += 1;
        }
    } else if ((ch & 0xF0) == 0xE0) {
        event->type = PITCH_BEND_EVENT;
        if (value1 < 0x80) {
            event->value1 = value1;
            bytes_read--;
        } else {
            event->value1 = getc(file); // Least significant bits
        }
        event->value2 = getc(file);     // Most significant bits
        bytes_read += 2;
    }

    return bytes_read;
}

void load_track_chunk(FILE* file, Track* track) {
    byte buffer[8];
    fread(track->chunk_type, 4, 1, file);
    track->chunk_type[4] = 0;
    fread(buffer, 4, 1, file);
    track->length = load_bytes(buffer, 4);
    int byte_count = 0;
    int i;
    byte previous = 0x00;
    for (i = 0; byte_count < track->length; i++) {
        byte_count += load_track_event(file, &track->events[i], previous);
        previous = track->events[i].type_code;
    }
    track->events_length = i;
}

void load_midi_file(FILE* file, MidiFile* contents) {
    // Header chunk
    load_header_chunk(file, &contents->header);

    // TODO throw error if header chunk's track count is greater than MAX_TRACKS

    // Track chunks
    for (int i = 0; i < contents->header.track_count; i++) {
        load_track_chunk(file, &contents->tracks[i]);
    }
}

