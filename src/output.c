#include <stdio.h>

#include "util.h"
#include "music.h"
#include "output.h"

// These are in order, make sure indices line up with EventType enum values.
char* event_types[] = {
    "Unset",
    "Note Off Event",
    "Note On Event",
    "Note Aftertouch Event",    // Note pressure change
    "Controller Event",
    "Program Change Event",     // Instrument change
    "Channel Aftertouch Event", // Channel pressure change
    "Pitch Bend Event",         // Channel pitch change
    "Sysex Event",              // System exclusive event
    "Meta Event"
};

char* get_event_type_string(EventType type) {
    return event_types[type];
}

char* meta_types[] = {
    "Non-Applicable",   // Not a meta event
    "Sequence Number",
    "Text Event",
    "Copyright Notice",
    "Track Name",       // Or sequence name in a format 0 file
    "Instrument Name",
    "Lyric",
    "Marker",
    "Cue Point",
    "MIDI Channel Prefix",
    "End of Track",
    "Set Tempo",
    "SMPTE Offset",
    "Time Signature",
    "Key Signature",
    "Sequencer-Specific"
};

char* get_meta_type_string(MetaType type) {
    return meta_types[type];
}

void print_header_chunk(Header* header) {
    printf("Header Chunk: %s, length %u\n", header->chunk_type, header->length);
    printf("Format: %hu  Track Count: %hu  ", header->format, header->track_count);
    printf("Division: %hu\n", header->division);
    printf("\n");
}

void print_track_chunk(Track* track, byte track_number) {
    printf("Track Chunk #%d: %s, length %d\n", track_number, track->chunk_type, track->length);
    for (int i = 0; i < track->events_length; i++) {
        TrackEvent event = track->events[i];
        printf("Event %d:\n", i + 1);
        char* type_string = get_event_type_string(event.type);
        printf("Delta Time - %hu  Type: %s\n", event.delta_time, type_string);
        // TODO add code to load and print event and meta types properly
        // (including MIDI event values)
        if (event.type == META_EVENT) {
            char* meta_type_string = get_meta_type_string(event.meta_type);
            printf("Meta Type: %s  Length: %d\n", meta_type_string, event.meta_length);
            if (event.meta_type == CHANNEL_PREFIX) {
                printf("Value: %d\n", event.metadata[0]);
            } else if (event.meta_type == SET_TEMPO) {
                int tempo = load_bytes(event.metadata, 3);
                printf("Value: %d\n", tempo);
            } else if (event.meta_type == TIME_SIGNATURE) {
                byte* values = event.metadata;
                printf("Values: %d %d %d %d\n", values[0], values[1], values[2], values[3]);
            } else if (event.meta_type == SEQUENCER_SPECIFIC) {
                printf("Metadata: ");
                for (int i = 0; i < event.meta_length; i++) {
                    printf("%02X ", event.metadata[i]);
                    if (i % 8 == 7) {
                        printf("\n");
                    }
                }
                printf(" ASCII: %s\n", event.metadata);
            } else if (event.meta_type != END_OF_TRACK) {
                printf("Metadata: %s\n", event.metadata);
            }
        } else if (event.type == NOTE_ON || event.type == NOTE_OFF) {
            printf("Note: %s  Velocity: %d\n", get_note_name(event.value1), event.value2);
        } else if (event.type == PROGRAM_CHANGE) {
            printf("Instrument: %s\n", get_instrument_name(event.value1));
        // TODO add option to display controller event types
        // Details here: https://www.mixagesoftware.com/en/midikit/help/HTML/controllers.html
        } else {
            printf("Value 1: %d  Value 2: %d\n", event.value1, event.value2);
        }
    }
    printf("\n");
}

void print_hex_data(FILE* file) {
    byte buffer[8];
    int len;
    while(len = fread(buffer, 1, sizeof(buffer), file)) {
        for (int i = 0; i < len; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\"");
        for (int i = 0; i < len; i++) {
            printf("%c", buffer[i]);
        }
        printf("\" ");
        printf("\n");
    }
}

