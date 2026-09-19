#ifndef STRUCT_H
#define STRUCT_H

#include "config.h"

#define TRUE 1
#define FALSE 0
#define true TRUE
#define false FALSE

typedef unsigned char bool;

typedef unsigned char byte;

typedef enum {
    UNSET,              // default, 0x00
    NOTE_OFF,           // 0x8
    NOTE_ON,            // 0x9
    NOTE_AFTERTOUCH,    // 0xA
    CONTROLLER_EVENT,   // 0xB
    PROGRAM_CHANGE,     // 0xC
    CHANNEL_AFTERTOUCH, // 0xD
    PITCH_BEND_EVENT,   // 0xE
    SYSEX_EVENT,        // 0xF0 or 0xF7
    META_EVENT          // 0xFF
} EventType;

typedef enum {
    N_A,                // Not a meta-event
    SEQUENCE_NUMBER,    // 0x00
    TEXT_EVENT,         // 0x01
    COPYRIGHT,          // 0x02
    TRACK_NAME,         // 0x03
    INSTRUMENT_NAME,    // 0x04
    LYRIC,              // 0x05
    MARKER,             // 0x06
    CUE_POINT,          // 0x07
    CHANNEL_PREFIX,     // 0x20
    END_OF_TRACK,       // 0x2F
    SET_TEMPO,          // 0x51
    SMPTE_OFFSET,       // 0x54
    TIME_SIGNATURE,     // 0x58
    KEY_SIGNATURE,      // 0x59
    SEQUENCER_SPECIFIC  // 0x7F
} MetaType;

typedef struct {
    int delta_time;
    EventType type;
    byte type_code;
    byte value1;
    byte value2;
    MetaType meta_type;
    byte meta_type_code;
    int meta_length;
    byte metadata[MAX_METADATA_LENGTH];
} TrackEvent;

typedef struct {
    char chunk_type[5];     // MThd
    unsigned int length;    // Should always be 6
    // Data portion
    unsigned short int format;
    unsigned short int track_count;
    unsigned short int division;
} Header;

typedef struct {
    byte chunk_type[5]; // MTrk
    unsigned int length;
    TrackEvent events[MAX_TRACK_EVENTS];
    unsigned int events_length;
} Track;

typedef struct {
    Header header;
    Track tracks[MAX_TRACKS]; // See length in header.track_count
} MidiFile;

// Song Data
typedef enum {
    SIXTY_FOURTH,
    THIRTY_SECOND,
    SIXTEENTH,
    EIGHTH,
    QUARTER,
    HALF,
    WHOLE,
    DOUBLE,
    TRIPLE,
    QUADRUPLE
} Frequency;

typedef struct {
    byte value;
    Frequency frequency;
    bool dotted;
} Note;

typedef struct {
    Frequency frequency;
    bool dotted;
} Rest;

typedef struct {
    byte note_values[MAX_CHORD_NOTES];
    byte chord_note_count;
    Frequency frequency;
    bool dotted;
} Chord;

typedef union {
    Note note;
    Rest rest;
    Chord chord;
} SongEvent;

typedef struct {
    byte instrument;
    byte note_count;
    SongEvent notes[MAX_NOTES];
} SongTrack;

typedef struct {
    int track_count;
    // TODO: add support for different time signatures
} MidiConfig;

typedef struct {
    MidiConfig config;
    SongTrack tracks[MAX_TRACKS];
} MidiSong;

#endif

