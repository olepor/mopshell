#include "parser.h"
#include <stdlib.h>

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
 * Takes the input command and parses it into an array of arrays,
 * all null-terminated for traversing.
 * @param input - input string
 * @return char*** parsed_input - An array of arrays to the parsed tokens
 */
char*** parse_input(char* input) {
  // First split by the pipe seperators
  int pipes = count_delimeters(input, "|");
  return NULL;
}
