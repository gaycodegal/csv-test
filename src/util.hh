#pragma once

#include <string>
#include <optional>
#include <sstream>

extern int MIDI_ERROR;

/**
 * takes in things like "C-2#"
 * C-2# -> (1)
 * A4b -> (80)
 * returns between 0 - 127
 * 
 */
int string_to_midi(std::string note);

/**
 * Parses ints, if fail, return std::nullopt
 */
std::optional<int> parse_int(std::string s);

