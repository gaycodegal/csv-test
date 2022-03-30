#include "util.hh"
int MIDI_ERROR = 0;

char note_values[] = {
  9, //A
  11,//B
  0, //C
  2, //D
  4, //E
  5, //F
  7  //G
};

/*
 * Takes in "C#-2"
 */
int string_to_midi(std::string note) {
  int midi = 24; // C0 is 24
  int note_index = 0;
  if (note.length() < 2) {
    return MIDI_ERROR;
  }

  // handle the base note
  // A-G -> 0-6; a-g -> 0-6
  // A < a
  int c = static_cast<int>(note[note_index++]);
  c = c < 'a' ? (c - 'A') : (c - 'a');
  if (c < 0 || c > 6) {
    return MIDI_ERROR;
  }

  // Sharp and flat
  if (note[note_index] == '#') {
    ++note_index;
    midi += 1;
  } else if(note[note_index] == 'b') {
    ++note_index;
    midi -= 1;
  }

  // negative octaves
  int multiplier = 1;
  if (note[note_index] == '-') {
    if (note.length() < note_index + 1) {
      return MIDI_ERROR;
    }
    ++note_index;
    multiplier = -1;
  }

  // both positive and negative octaves
  // should be 0-8
  int octave = static_cast<int>(note[note_index] - '0');
  if (octave < 0 || octave > 8 || (multiplier == -1 && octave > 2)) {
    return MIDI_ERROR;
  }
  
  midi += note_values[c] + octave * 12 * multiplier;

  // bounds check for things like A8
  if (midi > 127) {
    return 127;
  }
  // bounds check Cb-2
  if (midi < 0) {
    return 0;
  }
  
  return midi;
}

std::optional<int> parse_int(std::string s) {
  char c;
  std::stringstream ss(s);
  int i;
  ss >> i;
  if (ss.fail() || ss.get(c)) {
    return std::nullopt;
  }
  return {i};
}
