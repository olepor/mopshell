#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Utility for checking memeory allocation
 */
#define check_mem(memptr, errormessage)         \
  if (memptr == NULL) perror(errormessage)

/**
 * @param str - Input string to be checked for delimeters
 * @param delim - The delimeter to be counted
 * @return delims - The number of delimeters in string
 */
int count_delimeters(char* str, char* delim) {
  int delims = 0;
  for(char* c=str; *c!=0; c++) {
    if (*c == *delim) {
      delims+=1;
    }
  }
  return delims;
}

/**
 * Splits a string into an array of strings and terminates it with a NULL pointer.
 * Beware, the original string will be destroyed!
 * @param string - the input string to be split. Caution! this is naively
 * expected to be a valid null-terminated string.
 * @param path_delim - string_delim(eter)
 * @return string_arr - a malloc'd string array of size strlen(string)
 */
char** split_string(char* string, char* string_delim) {
  char** string_arr = (char**) calloc(strlen(string), sizeof(char));
  check_mem(string_arr, "Failed to allocate memory for string_arr");
  char* _str = strtok(string, string_delim);
  int i=0;
  for(; _str != NULL; _str = strtok(NULL, string_delim)) {
    /* printf("_str is: %s\n", _str); */
    if (*_str == '\n') {
      printf("end of line");
      break;
    }
    string_arr[i++] = _str;
  }
  string_arr[i]=NULL;
  return string_arr;
}

/**
 * Takes the input command and parses it into an array of arrays,
 * all null-terminated for traversing.
 * @param input - input string
 * @return char*** parsed_input - An array of arrays to the parsed tokens
 */
char*** parse_input(char* input) {
  char*** commands;
  // First split by the pipe seperators
  int pipes = count_delimeters(input, "|");
  // allocate memory for the commands-pointer array
  commands = calloc(pipes+1, sizeof(char*)); // +1 for NULL

  return NULL;
}
