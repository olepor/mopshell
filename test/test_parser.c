/* file minunit_example.c */

#include <stdio.h>
#include "../src/parser.h"
#include "minunit.h"

int tests_run = 0;

static char * test_count_delimeters() {
  const char* tst_str1 = "echo hello\n";
  const char* tst_str2 = "echo hello | rev";
  const char* tst_str3 = "echo hello rev rev";
  const char* tst_str4 = "echohellorevrev";
  mu_assert("error, count_delimeters!", count_delimeters((char*)tst_str1, " ") == 1);
  mu_assert("error, count_delimeters!", count_delimeters((char*)tst_str2, "|") == 1);
  mu_assert("error, count_delimeters!", count_delimeters((char*)tst_str3, " ") == 3);
  mu_assert("error, count_delimeters!", count_delimeters((char*)tst_str4, " ") == 0);
  return 0;
}

static char * test_split_string(char* string, char* string_delim) {
  const char* tst_str1 = "echo hello\n";
  return 0;
}

static char * all_tests() {
  mu_run_test(test_count_delimeters);
  return 0;
}

int main(int argc, char **argv) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
 }
