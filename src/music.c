
#include "struct.h"
#include "music.h"

// MIDI notes in order by note number (from lowest to highest)
char* notes[] = {
    "C-1",      // 0
    "Db-1",
    "D-1",
    "Eb-1",
    "E-1",
    "F-1",
    "Gb-1",
    "G-1",
    "Ab-1",
    "A-1",
    "Bb-1",     // 10
    "B-1",
    "C0",
    "Db0",
    "D0",
    "Eb0",
    "E0",
    "F0",
    "Gb0",
    "G0",
    "Ab0",      // 20
    "A0",
    "Bb0",
    "B0",
    "C1",
    "Db1",
    "D1",
    "Eb1",
    "E1",
    "F1",
    "Gb1",      // 30
    "G1",
    "Ab1",
    "A1",
    "Bb1",
    "B1",
    "C2",
    "Db2",
    "D2",
    "Eb2",
    "E2",       // 40
    "F2",
    "Gb2",
    "G2",
    "Ab2",
    "A2",
    "Bb2",
    "B2",
    "C3",
    "Db3",
    "D3",       // 50
    "Eb3",
    "E3",
    "F3",
    "Gb3",
    "G3",
    "Ab3",
    "A3",
    "Bb3",
    "B3",
    "C4",       // 60, Middle C
    "Db4",
    "D4",
    "Eb4",
    "E4",
    "F4",
    "Gb4",
    "G4",
    "Ab4",
    "A4",
    "Bb4",      // 70
    "B4",
    "C5",
    "Db4",
    "D4",
    "Eb4",
    "E4",
    "F4",
    "Gb4",
    "G4",
    "Ab5",      // 80
    "A5",
    "Bb5",
    "B5",
    "C6",
    "Db6",
    "D6",
    "Eb6",
    "E6",
    "F6",
    "Gb6",      // 90
    "G6",
    "Ab6",
    "A6",
    "Bb6",
    "B6",
    "C7",
    "Db7",
    "D7",
    "Eb7",
    "E7",       // 100
    "F7",
    "Gb7",
    "G7",
    "Ab7",
    "A7",
    "Bb7",
    "B7",
    "C8",
    "Cb8",
    "D8",       // 110
    "Eb8",
    "E8",
    "F8",
    "Gb8",
    "G8",
    "Ab8",
    "A8",
    "Bb8",
    "B8",
    "C9",       // 120
    "Db9",
    "D9",
    "Eb9",
    "E9",
    "F9",
    "Gb9",
    "G9",       // 127 (final MIDI note in most cases)
    "Ab9"
};

char* get_note_name(byte note_number) {
    return notes[note_number];
}

// MIDI instruments in order by program number
char* instruments[] = {
    "Grand Piano",          // 0, Piano
    "Bright Piano",
    "Electric Grand",
    "Honky-tonk",
    "Electric Piano 1",
    "Electric Piano 2",
    "Harpsichord",
    "Clavinet",
    "Celesta",              // Chromatic Percussion
    "Glockenspiel",
    "Music Box",            // 10
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "Tubular Bells",
    "Dulcimer",
    "Drawbar Organ",        // Organ
    "Percussive Organ",
    "Rock Organ",
    "Church Organ",
    "Reed Organ",           // 20
    "Accordion",
    "Harmonica",
    "Tango Accordion",
    "Acoustic Guitar",      // Guitar
    "Steel-string Guitar",
    "Jazz Guitar",
    "Electric Guitar",
    "Muted Guitar",
    "Overdrive Guitar",
    "Distortion Guitar",    // 30
    "Guitar Harmonics",
    "Acoustic Bass",        // Bass
    "Fingered Bass",
    "Picked Bass",
    "Fretless Bass",
    "Slap Bass 1",
    "Slap Bass 2",
    "Synth Bass 1",
    "Synth Bass 2",
    "Violin",               // 40, Strings
    "Viola",
    "Cello",
    "Contrabass",
    "Tremolo Strings",
    "Pizzicato Strings",
    "Orchestral Harp",
    "Timpani",
    "Strings 1",            // Ensemble
    "Strings 2",
    "Synth Strings 1",      // 50
    "Synth Strings 2",
    "Choir Aahs",
    "Voice Oohs",
    "Synth Voice",
    "Orchestra Hit",
    "Trumpet",              // Brass
    "Trombone",
    "Tuba",
    "Muted Trumpet",
    "French Horn",          // 60
    "Brass Section",
    "Synth Brass 1",
    "Synth Brass 2",
    "Soprano Sax",          // Reed
    "Alto Sax",
    "Tenor Sax",
    "Baritone Sax",
    "Oboe",
    "English Horn",
    "Bassoon",              // 70
    "Clarinet",
    "Piccolo",              // Pipe
    "Flute",
    "Recorder",
    "Pan Flute",
    "Blown Bottle",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    "Square Lead",          // 80, Synth Lead
    "Sawtooth Lead",
    "Calliope Lead",
    "Chiff Lead",
    "Charang Lead",
    "Voice Lead",
    "Fifth Lead",
    "Bass & Lead",
    "New Age Pad",          // Synth Pad
    "Warm Pad",
    "Polysynth Pad",        // 90
    "Choir Pad",
    "Bowed Pad",
    "Metallic Pad",
    "Halo Pad",
    "Sweep Pad",
    "Rain",                 // Synth Effects
    "Soundtrack",
    "Crystal",
    "Atmosphere",
    "Brightness",           // 100
    "Goblins",
    "Echoes",
    "Sci-Fi",
    "Sitar",                // Ethnic Instruments
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "Bagpipe",
    "Fiddle",               // 110
    "Shanai",
    "Tinkle Bell",          // Percussive Instruments
    "Agogo",
    "Steel Drums",
    "Woodblock",
    "Taiko Drum",
    "Melodic Tom",
    "Synth Drum",
    "Reverse Cymbal",
    "Guitar Fret Noise",    // 120, Sound Effects
    "Breath Noise",
    "Seashore",
    "Bird Tweet",
    "Telephone",
    "Helicopter",
    "Applause",
    "Gunshot",              // 127, last MIDI instrument
};

char* get_instrument_name(byte program_number) {
    return instruments[program_number];
}

