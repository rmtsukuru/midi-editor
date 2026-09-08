#include <stdio.h>

#include "src/struct.h"
#include "src/file_input.h"
#include "src/file_output.h"
#include "src/output.h"

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
    MidiFile* data;
    load_default_midi_data(data);
    save_midi_file(file, data);
    fclose(file);
    printf("Saved file %s to disk.\n", filename);

    return 0;
}

int main(int argc, char** argv) {
    if (argc > 1) {
        return display_midi_file(argv[1]);
    }

    printf("No MIDI file provided. Create new file? [Yn]\n>");
    char selection = getchar();
    if (selection == 'n' || selection == 'N') {
        return 0;
    }

    return create_midi_file();
}

