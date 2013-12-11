#include <stdlib.h>
#include <stdio.h>
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
    freopen(test_file, "r", input);
    yyrestart(input);
    ASSERT_EQ(expected_token, yylex());
    ASSERT_STREQ(expected_string.c_str(), yytext);
  }

  void assert_newline() {
    assert_token((int)'\n', "\n");
  }

  void assert_warning() {
    // TODO make assertion for flex warning here
  }

  virtual ~ScannerTest() {
    fclose(input);
    remove(test_file);
  }

  FILE* input = NULL;
  char* test_file = "/tmp/scanner_test_input";
};


TEST_F(ScannerTest, integer_single_digit) {
  scan("0");
  assert_token(INTEGER, "0");
}

TEST_F(ScannerTest, integer_multiple_digit) {
  scan("123");
  assert_token(INTEGER, "123");
}

TEST_F(ScannerTest, real_with_following_dow) {
  scan("123.");
  assert_token(REAL, "123.");
}

TEST_F(ScannerTest, real_with_leading_dot) {
  scan(".123");
  assert_token(REAL, ".123");
}

TEST_F(ScannerTest, real) {
  scan("123.123");
  assert_token(REAL, "123.123");
}

TEST_F(ScannerTest, real_with_exponent_downcased) {
  scan("1.1e21");
  assert_token(REAL, "1.1e21");
}

TEST_F(ScannerTest, real_with_exponent_upcased) {
  scan("1.1E21");
  assert_token(REAL, "1.1E21");
}

TEST_F(ScannerTest, real_with_negative_exponent) {
  scan("1.1E-21");
  assert_token(REAL, "1.1E-21");
}

TEST_F(ScannerTest, real_with_positive_exponent) {
  scan("1.1E+21");
  assert_token(REAL, "1.1E+21");
}

TEST_F(ScannerTest, real_with_following_dot_and_exponent) {
  scan("1.E21");
  assert_token(REAL, "1.E21");
}

TEST_F(ScannerTest, real_with_following_dot_and_negative_exponent) {
  scan("1.E-21");
  assert_token(REAL, "1.E-21");
}

TEST_F(ScannerTest, real_with_following_dot_and_positive_exponent) {
  scan("1.E+21");
  assert_token(REAL, "1.E+21");
}

TEST_F(ScannerTest, real_with_leading_dot_and_exponent) {
  scan(".1E21");
  assert_token(REAL, ".1E21");
}

TEST_F(ScannerTest, real_with_leading_dot_and_negative_exponent) {
  scan(".1E-21");
  assert_token(REAL, ".1E-21");
}

TEST_F(ScannerTest, real_with_leading_dot_and_positive_exponent) {
  scan(".1E+21");
  assert_token(REAL, ".1E+21");
}

TEST_F(ScannerTest, integer_with_exponent) {
  scan("1E20");
  assert_token(REAL, "1E20");
}

TEST_F(ScannerTest, integer_with_negative_exponent) {
  scan("1E-20");
  assert_token(REAL, "1E-20");
}

TEST_F(ScannerTest, integer_with_positive_exponent) {
  scan("1E+20");
  assert_token(REAL, "1E+20");
}

TEST_F(ScannerTest, dot) {
  scan(".");
  assert_token((int)'.', ".");
}

TEST_F(ScannerTest, identifier) {
  scan("ABC");
  assert_token(ID, "ABC");
}

TEST_F(ScannerTest, identifier_with_number) {
  scan("ABC123");
  assert_token(ID, "ABC123");
}

TEST_F(ScannerTest, identifier_with_E) {
  scan("E2");
  assert_token(ID, "E2");
}

TEST_F(ScannerTest, identifiers_with_number_and_underscore) {
  scan("ABC_123");
  assert_token(ID, "ABC_123");
}

TEST_F(ScannerTest, identifier_with_numbers_and_underscores) {
  scan("ABC_123_ABC");
  assert_token(ID, "ABC_123_ABC");
}

TEST_F(ScannerTest, integer_and_identifier) {
  scan("123ABC");
  assert_token(INTEGER, "123");
  assert_token(ID, "ABC");
}

TEST_F(ScannerTest, leading_underscore_and_identifier) {
  scan("_ABC123_");
  assert_token((int)'_', "_");
  assert_token(ID, "ABC123_");
}

TEST_F(ScannerTest, cpp_comment){
  scan("1  // Here is a C++ style comment\n");
  assert_token(INTEGER, "1");
  assert_newline();
}

TEST_F(ScannerTest, cpp_comment_with_slashes) {
  scan("2  // Here is a C++ style comment with a // inside of it\n");
  assert_token(INTEGER, "2");
  assert_newline();
}

TEST_F(ScannerTest, cpp_comment_with_multiline) {
  scan("3  // Here is one with a C style /* comment */ inside\n");
  assert_token(INTEGER, "3");
  assert_newline();
}

TEST_F(ScannerTest, c_comment) {
  scan("4  /* Here is a C style comment */\n");
  assert_token(INTEGER, "4");
  assert_newline();
}

TEST_F(ScannerTest, c_comment_with_slashes) {
  scan("5  /* Here is a C style comment with a // inside of it */\n");
  assert_token(INTEGER, "5");
  assert_newline();
}

TEST_F(ScannerTest, multiline_comment) {
  scan("6  /* This is a\n  multiline\n  comment */\n");
  assert_token(INTEGER, "6");
  assert_newline();
}

TEST_F(ScannerTest, multiline_comment_inside_multiline) {
  scan("7  /* This should cause /* a warning */\n");
  assert_token(INTEGER, "7");
  assert_newline();
  assert_warning();
}

TEST_F(ScannerTest, multiline_comment_inside_multiline_with_tokens) {
  scan("8  /* This should casue /* a warning */ and five tokens */\n");
  assert_token(INTEGER, "8");
  // assert nothing (comment)
  assert_warning();
  assert_token(AND, "and");
  assert_token(ID, "five");
  assert_token(ID, "tokens");
  assert_token((int)'*', "*");
}

}
