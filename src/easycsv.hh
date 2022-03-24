#pragma once
#include <csv.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::function<void(std::vector<std::string>* row_items)>
    row_parser_callback;

class CSVParser {
 private:
  struct csv_parser* parser;
  long unsigned fields;
  long unsigned rows;
  int is_first_item = true;
  int is_first_line = true;
  int is_comment = false;
  std::vector<std::string>* row_items;
  row_parser_callback row_parser;

 public:
  unsigned char options = 0;

  CSVParser();
  ~CSVParser();

  void readFile(const std::string filename,
                std::vector<std::string>* expectedColumns,
                row_parser_callback row_parser);
  void setOptions(unsigned char options);

  // you shouldn't be calling these, but they need to be public
  // for the callback the way I wrote it. Better than exposing
  // the member variables at least.
  // These do have side effects, so don't go calling them
  inline void callback_item(void* string_pointer, size_t len);
  inline void callback_assemble_row(int c);
};
