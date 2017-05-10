// TODO - clean includes
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <unistd.h>
#include "parser.h"

#define check_mem(memptr, errormessage)         \
  if (memptr == NULL) perror(errormessage)

/**
 * Return the length of a null-terminated array
 */
int arraylen(char** str_arr) {
  int i=0;
  for (;str_arr[i]!=NULL; i++);
  return i;
}

/**
 * Simply blocks and reads a line from stdin
 * using getline.
 */
char* get_user_input() {
  char* usr_input=NULL;
  size_t bufsize=0;
  getline(&usr_input, &bufsize, stdin);
  if (*usr_input == '\n')
    return NULL;
  return usr_input;
}

/**
 * chains the filedescriptors in a pipe accordingly
 * @param int oldfd - The old filedescriptor
 * @param int newfd - The new filedescriptor
 */
void redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd, newfd) != -1)
      close(oldfd); /* successfully redirected */
    else {
      perror("redirect failed!");
      exit(EXIT_FAILURE);
    }
  }
}

enum PIPE_TYPE {
  READ, WRITE
};

/**
 * Chains multiple pipes together
 * @param char*** commands - the command arrays
 * @return int in - the file descriptor to chain on
 */
int chain_commands(char*** commands) {
  int nr_commands = arraylen((char**)commands);
  int i = 0, in = STDIN_FILENO; /* the first command reads from stdin */
  for ( ; i < (nr_commands-1); ++i) {
    int fd[2]; /* in/out pipe ends */
    pid_t pid; /* child's pid */
    if (pipe(fd) == -1) {
      perror("noo");
      exit(1);
    }
    switch((pid=fork())) {

    case 0:
      close(fd[0]); /* close unused read end of the pipe */
      redirect(in, STDIN_FILENO);   /* <&in  : child reads from in */
      redirect(fd[WRITE], STDOUT_FILENO); /* >&out : child writes to out */
      execvp(commands[i][0], commands[i]);
      perror(commands[i][0]);
      exit(EXIT_FAILURE);
      break;

    case -1:
      perror("no fork!");
      exit(1);
      break;

    default:
      close(fd[WRITE]); /* close unused write end of the pipe */
      close(in);    /* close unused read end of the previous pipe */
      in = fd[READ]; /* the next command reads from here */
      break;

    }
  }
  return in;
}

/**
 * Checks for an ampersand in last position of the array
 * @param char** command - The parsed command array
 * @return true or false
 */
int run_in_background(char** command) {
  int end = arraylen(command)-1;
  if ((*command[end]) == '&') {
    command[end] = NULL; // Remove the ampersand
    return 1;
  }
  return 0;
}

/**
 * Runs the parsed input commands
 * @param char*** commands - The parsed input
 * @ return TODO
 */
void execute_commands(char*** commands) {
  int nr_commands = arraylen((char**)commands);
  int tmpin = dup(0);
  int tmpout = dup(1);
  // Run all the children
  int background = 0;
  background = run_in_background(commands[nr_commands-1]); // Remove the ampersand
  int in=0;
  if (nr_commands > 0)
    in = chain_commands(commands);
  // Run the last process (end of the pipe)
  pid_t pid;
  int fd[2];
  pipe(fd);
  if ((pid = fork()) == 0){
    close(fd[READ]);
    redirect(in, STDIN_FILENO);
    execvp(commands[nr_commands-1][0], commands[nr_commands-1]);
    perror(commands[nr_commands-1][0]);
    exit(EXIT_FAILURE);
  }
  int status;
  if (background) {
    while ((pid = wait(&status)) != -1) {
      fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
    }
  }
  // Reestablish stdin & stdout
  dup2(tmpin, 0);
  dup2(tmpout,1);
}


void run_terminal () {

  char*** commands;

  do {
    printf(">> ");
    char* input = get_user_input();
    if (input == NULL) { // return
      continue;
    } else {
      commands = parse_input(input);
    }
    if (strncmp(commands[0][0], "exit", 5) == 0) {
      exit(EXIT_SUCCESS);
    }
    execute_commands(commands);

  } while(1);

}

int main(void) {
  run_terminal();
}
