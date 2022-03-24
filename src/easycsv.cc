#include "easycsv.hh"

// declare callbacks; static to not pollute the namespace
static void cb1_item (void *p, size_t len, void *data);
static void cb2_assemble_row (int c, void *data);
static int is_space(unsigned char c);
static int is_term(unsigned char c);

CSVParser::CSVParser() {
  parser = new csv_parser();
  if (csv_init(parser, this->options) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    exit(EXIT_FAILURE);
  }

  csv_set_space_func(parser, is_space);
  csv_set_term_func(parser, is_term);
}

CSVParser::~CSVParser() {
  csv_free(this->parser);
}

void CSVParser::setOptions(unsigned char options) {
  csv_set_opts(parser, options);
}

void CSVParser::readFile
(const std::string filename,
 std::vector<std::string> *expected_columns,
 row_parser_callback row_parser) {
  printf("%s \n", filename.c_str());
  this->row_parser = row_parser;
  printf("%s \n", filename.c_str());

  FILE *fp;
  char buf[1024];
  size_t bytes_read;
  rows = 0;
  fields = 0;
  is_first_item = true;
  is_first_line = true;
  is_comment = false;
  printf("%s \n", filename.c_str());
  row_items = new std::vector<std::string>(expected_columns->size());
  fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    fprintf(stderr, "Failed to open %s: %s\n", filename.c_str(), strerror(errno));
    return;
  }

  while ((bytes_read=fread(buf, 1, 1024, fp)) > 0) {
    if (csv_parse(parser, buf, bytes_read, cb1_item, cb2_assemble_row, (void *)this) != bytes_read) {
      fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(parser)));
    }
  }

  csv_fini(parser, cb1_item, cb2_assemble_row, (void *)this);

  if (ferror(fp)) {
    fprintf(stderr, "Error while reading file %s\n", filename.c_str());
    fclose(fp);
    return;
  }
  
  fclose(fp);
  printf("%s: %lu fields, %lu rows\n", filename.c_str(), this->fields, this->rows);
  
}

inline void CSVParser::callback_item(void *string_pointer, size_t len){
  char *current_field = (char*)string_pointer;
  current_field[len] = '\0';
  // ignore rows starting with #, as they are comments
  if (this->is_first_item) {
    if (current_field[0] == '#') {
      this->is_comment = true;
    }
  }
  if (!this->is_comment) {
    printf("found: '%s'\n", current_field);
    fflush(stdin);
    printf("hi\n");
    if (fields < row_items->size()) {
      // copy constructor, should be ok
      fflush(stdin);
      (*row_items)[fields] = std::string(current_field);
      printf("%lu \n", row_items->size());
    }
    this->fields++;
  }
}

inline void CSVParser::callback_assemble_row(int c){
  if (!is_comment) {
    row_parser(row_items);
    rows++;
    is_first_line = false;
  }
  fields = 0;
  is_first_item = true;
  is_comment = false;
}

// wrapper around inline function callback_item
static void cb1_item (void *p, size_t len, void *data) {
  ((CSVParser *)data)->callback_item(p, len);
}

// wrapper around inline function callback_assemble_row
static void cb2_assemble_row (int c, void *data) {
  ((CSVParser *)data)->callback_assemble_row(c);
}

static int is_space(unsigned char c) {
  if (c == CSV_SPACE || c == CSV_TAB) return 1;
  return 0;
}

static int is_term(unsigned char c) {
  if (c == CSV_CR || c == CSV_LF) return 1;
  return 0;
}

