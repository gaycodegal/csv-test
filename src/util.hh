#pragma once

#include <string>

/**
 * takes in things like "C-2#"
 * C-2# -> (1)
 * A4b -> (80)
 * returns between 0 - 127
 * 
 */
int string_to_midi(std::string note);
extern int MIDI_ERROR;
