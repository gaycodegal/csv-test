#pragma once
#include <csv.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::function<void(std::vector<std::string> row_items)>
    row_parser_callback;

class CSVParser {
 private:
  csv_parser parser;

  inline CSVParser(csv_parser p) : parser(p) {}

 public:
  static std::optional<CSVParser> create(unsigned char options);

  CSVParser(CSVParser&&) = default;

  ~CSVParser();

  void setOptions(unsigned char options);

  void readFile(FILE* file, std::vector<std::string> expectedColumns,
                row_parser_callback callback);
};
