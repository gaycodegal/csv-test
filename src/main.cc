/*
csvinfo - reads CSV data from input file(s) and reports the number
          of fields and rows encountered in each file
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <csv.h>
#include <stdbool.h>
#include <unordered_map>
#include <iostream>
#include "easycsv.hh"

int main (int argc, char *argv[]) {
  CSVParser p;

  if (argc < 2) {
    fprintf(stderr, "Usage: csvinfo [-s] files\n");
    exit(EXIT_FAILURE);
  }

  while (*(++argv)) {
    if (strcmp(*argv, "-s") == 0) {
      // doesn't do anything atm
      p.options = CSV_STRICT;
      continue;
    }
    auto row_parser = [](std::vector<std::string>* expectedColumns){
      std::cout << expectedColumns << std::endl;
    };
    std::vector<std::string> expectedColumns{"code", "name"};
    p.readFile(*argv, &expectedColumns, row_parser);
  }
}
 
