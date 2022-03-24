/*
csvinfo - reads CSV data from input file(s) and reports the number
          of fields and rows encountered in each file
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

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: csvinfo [-s] files\n");
    exit(EXIT_FAILURE);
  }

  while (*(++argv)) {
    unsigned char options = 0;
    if (strcmp(*argv, "-s") == 0) {
      // doesn't do anything atm
      options = CSV_STRICT;
      continue;
    }

    if (auto p{CSVParser::create(options)}; !p) {
      std::cerr << "Failed to initialize csv parser\n";
    } else {
      std::string filename{*argv};

      FILE* fp = fopen(filename.c_str(), "rb");
      if (!fp) {
        std::cout << "Failed to open " << filename << ": " << strerror(errno)
                  << "\n";
        return EXIT_FAILURE;
      }

      p->readFile(fp, std::vector<std::string>{"code", "name"},
                  [](std::vector<std::string>* expectedColumns) {
                    std::cout << expectedColumns << "\n";
                  });

      if (ferror(fp)) {
        std::cerr << "Error while reading file " << filename << "\n";
        fclose(fp);
        return EXIT_FAILURE;
      }

      fclose(fp);
    }
  }
}
