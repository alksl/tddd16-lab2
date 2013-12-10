#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gtest/gtest.h>
#include <scanner.h>

extern  FILE* yyin;
extern  char* yytext;
extern  int yylex();
extern  void yyrestart(FILE*);

namespace {

class ScannerTest : public ::testing::Test {
protected:
  ScannerTest() {
    input = fopen(test_file, "w");
  }

  void scan(std::string input_string) {
    fprintf(input, "%s\n", input_string.c_str());
  }

  void assert_token(int expected_token, std::string expected_string) {
    int scanned_token;
    freopen(test_file, "r", input);
    yyrestart(input);
    ASSERT_EQ(expected_token, yylex());
    ASSERT_STREQ(expected_string.c_str(), yytext);
  }

  virtual ~ScannerTest() {
    fclose(input);
    remove(test_file);
  }

  FILE* input = NULL;
  char* test_file = "/tmp/scanner_test_input";
};


TEST_F(ScannerTest, _if) {
  scan("if");
  assert_token(IF, "if");
}

TEST_F(ScannerTest, _else) {
  scan("else");
  assert_token(ELSE, "else");
}

}
