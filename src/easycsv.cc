#include "easycsv.hh"

#include <iostream>
#include <string_view>
#include <span>

// declare callbacks; static to not pollute the namespace
static int is_space(unsigned char c) {
  if (c == CSV_SPACE || c == CSV_TAB) return 1;
  return 0;
}

static int is_term(unsigned char c) {
  if (c == CSV_CR || c == CSV_LF) return 1;
  return 0;
}

struct CSVInfo {
  std::vector<std::string> row_items;
  row_parser_callback row_parser;

  long unsigned fields = 0;
  long unsigned rows = 0;
  int is_first_item = true;
  int is_first_line = true;
  int is_comment = false;

  void callback_item(std::span<char> str);

  void callback_assemble_row(int c);

  inline static void cb1_item(void *str, size_t len, void *parser) {
    reinterpret_cast<CSVInfo *>(parser)->callback_item(
        std::span<char>{reinterpret_cast<char *>(str), len});
  };

  inline static void cb2_assemble_row(int c, void *parser) {
    reinterpret_cast<CSVInfo *>(parser)->callback_assemble_row(c);
  };
};

inline void CSVInfo::callback_item(std::span<char> str) {
  // ignore rows starting with #, as they are comments
  if (is_first_item) {
    if (!str.empty() && str[0] == '#') {
      is_comment = true;
    }
  }
  if (!is_comment) {
    if (fields < row_items.size()) {
      // copy constructor, should be ok
      row_items[fields] = std::string{str.begin(), str.end()};
    }
    fields++;
  }
}

inline void CSVInfo::callback_assemble_row(int c) {
  if (!is_comment) {
    if (!is_first_line) { 
      row_parser(row_items);
      rows++;
    }
    is_first_line = false;
  }
  fields = 0;
  is_first_item = true;
  is_comment = false;
}

std::optional<CSVParser> CSVParser::create(unsigned char options) {
  csv_parser parser;
  if (csv_init(&parser, options) != 0) return std::nullopt;

  csv_set_space_func(&parser, is_space);
  csv_set_term_func(&parser, is_term);

  return {CSVParser{parser}};
}

CSVParser::~CSVParser() { csv_free(&parser); }

void CSVParser::setOptions(unsigned char options) {
  csv_set_opts(&parser, options);
}

void CSVParser::readFile(FILE *file, std::vector<std::string> expected_columns,
                         row_parser_callback callback) {
  CSVInfo info{
      .row_items = std::move(expected_columns),
      .row_parser = callback,
  };

  char buf[1024];
  size_t bytes_read;
  while ((bytes_read = fread(buf, 1, 1024, file)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, &CSVInfo::cb1_item,
                  &CSVInfo::cb2_assemble_row, &info) != bytes_read) {
      std::cerr << "Error while parsing file: "
                << csv_strerror(csv_error(&parser)) << "\n";
    }
  }

  csv_fini(&parser, &CSVInfo::cb1_item, &CSVInfo::cb2_assemble_row, &info);

  std::cout << info.rows << " rows\n";
}
