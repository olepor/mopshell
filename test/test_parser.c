#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/parser.h"
#include "minunit.h"

int tests_run = 0;

static int arraylen(char** str_arr) {
  int i=0;
  for (;str_arr[i]!=NULL; i++);
  return i;
}

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

char* create_dyn_string(const char* str_lit) {
  char* dstr = calloc(strlen(str_lit)+1, sizeof(char));
  memcpy(dstr, str_lit, strlen(str_lit));
  return dstr;
}

static char * test_split_string() {
  char* tst_str1 = create_dyn_string("echo hello\n");
  char** ret_str = split_string(tst_str1, " ");
  mu_assert("error, split_string!", arraylen(ret_str) == 2);
  char* tst_str2 = create_dyn_string("echo hello | rev");
  mu_assert("error, split_string!", arraylen(split_string((char*)tst_str2, "|")) == 2);
  char* tst_str3 = create_dyn_string("onelonestring");
  mu_assert("error, split_string!", arraylen(split_string((char*)tst_str3, "|")) == 1);
  free(tst_str1); free(tst_str2); free(tst_str3);
  return 0;
}

static char* test_parse_input() {
  char* tst_str = create_dyn_string("echo hello\n");
  char*** commands = parse_input(tst_str);
  mu_assert("error, parse_input: 0:0", strcmp(commands[0][0], "echo") == 0);
  mu_assert("error, parse_input: 0:1", strcmp(commands[0][1], "hello") == 0);
  char** str = commands[0];
  mu_assert("error, parse_input: 1", commands[0][2] == NULL);
  free_command_mem(commands);
  /* Case 2 - several dimensions */
  char* tst_str2 = create_dyn_string("echo hello | rev\n");
  commands = parse_input(tst_str2);
  mu_assert("error, parse_input2: 0:0", strcmp(commands[0][0], "echo") == 0);
  mu_assert("error, parse_input2: 0:1", strcmp(commands[0][1], "hello") == 0);
  mu_assert("error, parse_input2: not null", commands[0][2] == NULL);
  mu_assert("error, parse_input2: 1:0", strcmp(commands[1][0], "rev") == 0);
  mu_assert("error, parse_input2: not null", commands[1][1] == NULL);
  free_command_mem(commands);
  char* tst_str4 = create_dyn_string("echo hello | rev | rev\n");
  commands = parse_input(tst_str4);
  mu_assert("error, parse_input2: 0:0", strcmp(commands[0][0], "echo") == 0);
  mu_assert("error, parse_input2: 0:1", strcmp(commands[0][1], "hello") == 0);
  mu_assert("error, parse_input2: not null", commands[0][2] == NULL);
  mu_assert("error, parse_input2: 1:0", strcmp(commands[1][0], "rev") == 0);
  mu_assert("error, parse_input2: not null", commands[1][1] == NULL);
  mu_assert("error, parse_input2: 1:0", strcmp(commands[2][0], "rev") == 0);
  mu_assert("error, parse_input2: not null", commands[2][1] == NULL);
  free_command_mem(commands);
  return 0;
}

static char * all_tests() {
  mu_run_test(test_count_delimeters);
  mu_run_test(test_split_string);
  mu_run_test(test_parse_input);
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
