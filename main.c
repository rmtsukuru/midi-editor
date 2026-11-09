#include <stdio.h>

#include "src/struct.h"
#include "src/file_input.h"
#include "src/file_output.h"
#include "src/music.h"
#include "src/output.h"
#include "src/util.h"

int display_hex_contents(char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Could not load file; most likely invalid filename.\n");
        return -1;
    }

    print_hex_data(file);
}

int display_midi_file(char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Could not load file; most likely invalid filename.\n");
        return -1;
    }

    MidiFile data;
    load_midi_file(file, &data);
    fclose(file);

    print_header_chunk(&data.header);
    for (int i = 0; i < data.header.track_count; i++) {
        print_track_chunk(&data.tracks[i], i + 1);
    }

    // Print out any remaining bytes in the file as raw hex
    // Should be none if everything was parsed correctly
    print_hex_data(file);
    return 0;
}

int create_midi_file() {
    printf("What is the name for this file?\n>");
    char filename[100];
    scanf(" %s", filename);
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        printf("Could not open file for writing; most likely invalid filename.\n");
        return -1;
    }

    MidiFile data;
    load_default_midi_data(&data);
    int byte_count = save_midi_file(file, &data);
    fclose(file);
    printf("Saved file %s to disk (%d bytes).\n", filename, byte_count);

    return 0;
}

int open_song_file() {
    printf("What is the name for this file?\n>");
    char filename[100];
    scanf(" %s", filename);
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open file for writing; check that filename is valid.\n");
        return -1;
    }

    MidiSong song_data;
    load_song_file(file, &song_data);
    MidiConfig* config = &song_data.config;
    printf("Track Count: %d\n", config->track_count);
    printf("Time Signature: %d/%d\n", config->time_sig_count, config->time_sig_unit);
    printf("Tempo: %d BPM\n", config->tempo);
    SongTrack* track = &song_data.tracks[0];
    printf("Track 1 Instrument: %s\n", get_instrument_name(track->instrument));
    for (int i = 0; i < track->note_count; i++) {
        if (track->notes[i].type == NOTE) {
            printf("%s ", get_note_name(track->notes[i].data.note.value));
        }
    }
    printf("\n");
    return 0;
}

int main(int argc, char** argv) {
    if (argc > 2) {
        return display_hex_contents(argv[1]);
    } else if (argc > 1) {
        return display_midi_file(argv[1]);
    }

    printf("No MIDI file provided. Create new file? [Yn]\n>");
    char selection = getcin();
    if (selection == 'n' || selection == 'N') {
        printf("Load song file for viewing? [Yn]\n>");
        selection = getcin();
        if (selection == 'n' || selection == 'N') {
            return 0;
        }
        return open_song_file();
    }

    return create_midi_file();
}

