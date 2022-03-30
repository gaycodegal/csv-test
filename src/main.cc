/*
  csv test - read in a keymap and store it in memory
*/

#include <csv.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>

#include "easycsv.hh"
#include "util.hh"

enum KeyMap { code, name };

int main(int argc, char* argv[]) {
  if (auto p{CSVParser::create(0)}; !p) {
    std::cerr << "Failed to initialize csv parser\n";
  } else {
    std::string filename = "./data/qwerty.csv";

    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
      std::cout << "Failed to open " << filename << ": " << strerror(errno)
                << "\n";
      return EXIT_FAILURE;
    }

    std::unordered_map<std::string, int> qwerty_map;

    p->readFile(fp, std::vector<std::string>{"code", "name"},
                [&qwerty_map](std::vector<std::string> row) {
                  /*for (auto &item: row) {
                    std::cout << item << ' ';
                    }
                    std::cout << std::endl;*/
                  if (auto code{parse_int(row[KeyMap::code])}; code) {
                    qwerty_map[row[KeyMap::name]] = code.value();
                  }
                });

    // check that qwerty map can read a = 65
    std::cout << "should be 65 (a): " << qwerty_map["a"] << std::endl;
    std::cout << "Ab4(80): " << string_to_midi("Ab4") << std::endl;
    std::cout << "C0(24): " << string_to_midi("C0") << std::endl;
    std::cout << "C-2(0): " << string_to_midi("C-2") << std::endl;
    std::cout << "F#2(54): " << string_to_midi("F#2") << std::endl;

    if (ferror(fp)) {
      std::cerr << "Error while reading file " << filename << "\n";
      fclose(fp);
      return EXIT_FAILURE;
    }

    fclose(fp);
  }
}
