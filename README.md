# midi-editor

Command line MIDI viewer and editor.

## Usage

Build by running `make`. This will generate `midi.exe` (should be `midi` on UNIX-based systems).

Run `./midi.exe filename.mid` to load and display contents of filename.mid.

Piping to `less` or using output redirection (`>`) to a file is recommended because the file contents tend to be long even for the simplest of MIDI files.

More details TBD, MIDI editing features are in progress.

