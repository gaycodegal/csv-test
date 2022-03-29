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
#include <sstream>
#include <unordered_map>

#include "easycsv.hh"

std::optional<int> parse_int(std::string s);
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

    if (ferror(fp)) {
      std::cerr << "Error while reading file " << filename << "\n";
      fclose(fp);
      return EXIT_FAILURE;
    }

    fclose(fp);
  }
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
