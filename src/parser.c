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
 * Beware, the original string will be destroyed cause of strtok
 * @param string - the input string to be split. Caution! this is naively
 * expected to be a valid null-terminated string.
 * @param path_delim - string_delim(eter)
 * @return string_arr - a calloc'd string array of size nr_delims+1
 */
char** split_string(char* string, char* string_delim) {
  int tokens = count_delimeters(string, string_delim);
  char** string_arr = (char**) calloc(tokens+1, sizeof(char*)); // +1 for NULL
  check_mem(string_arr, "Failed to allocate memory for string_arr");
  char* _str = strtok(string, string_delim);
  int i=0;
  for(; _str != NULL; _str = strtok(NULL, string_delim)) {
    if (*(_str+strlen(_str)-1) == '\n') {
      *(_str+strlen(_str)-1) = 0;
    }
    string_arr[i++] = _str;
  }
  string_arr[i]=NULL;
  return string_arr;
  return NULL;
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
  int nr_commands = count_delimeters(input, "|")+1;
  // allocate memory for the commands-pointer array
  commands = calloc(nr_commands+1, sizeof(char*)); // +1 for NULL
  char** command_arr = split_string(input, "|");
  for (int i=0; i<nr_commands; i++) {
    int tokens=count_delimeters(command_arr[i], " ");
    char** token_arr = split_string(command_arr[i], " ");
    commands[i] = token_arr;
  }

  return commands;
}
