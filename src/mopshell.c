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

#define Close(FD) do {                              \
    int Close_fd = (FD);                            \
    if (close(Close_fd) == -1) {                    \
      perror("close");                              \
      fprintf(stderr, "%s:%d: close(" #FD ") %d\n", \
              __FILE__, __LINE__, Close_fd);        \
    }                                               \
  }while(0)

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

/* move oldfd to newfd */
static void redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd, newfd) != -1)
      Close(oldfd); /* successfully redirected */
    else {
      perror("redirect failed!");
      exit(EXIT_FAILURE);
    }
  }
}

void run(char* const argv[], int in, int out) {
  redirect(in, STDIN_FILENO);   /* <&in  : child reads from in */
  redirect(out, STDOUT_FILENO); /* >&out : child writes to out */

  execvp(argv[0], argv);
  perror(argv[0]);
  exit(EXIT_FAILURE);
}

enum PIPE_TYPE {
  READ, WRITE
};

int chain_commands(char*** commands) {
  char*** command = commands;
  int nr_commands = arraylen(commands);
  int i = 0, in = STDIN_FILENO; /* the first command reads from stdin */
  for ( ; i < (nr_commands-1); ++i) {
    int fd[2]; /* in/out pipe ends */
    pid_t pid; /* child's pid */
    if (pipe(fd) == -1) {
      perror("noo");
      exit(1);
    }
    else if ((pid = fork()) == -1) {
      /* report_error_and_exit("fork"); */
      perror("no fork!");
      exit(1);
    }
    else if (pid == 0) { /* run command[i] in the child process */
      Close(fd[0]); /* close unused read end of the pipe */
      run((char* const*)command[i], in, fd[1]); /* $ command < in > fd[1] */
    }
    else { /* parent */
      assert (pid > 0);
      Close(fd[WRITE]); /* close unused write end of the pipe */
      Close(in);    /* close unused read end of the previous pipe */
      in = fd[READ]; /* the next command reads from here */
    }
  }
  return in;
}

int execute_commands(char*** commands) {

  const char*** command = commands;
  int nr_commands = arraylen(commands);
  int n = sizeof(command) / sizeof(*command);
  int tmpin = dup(0);
  int tmpout = dup(1);
  // Run all the children
  int in=0;
  if (nr_commands > 0)
    in = chain_commands(commands);
  /* int i = 0, in = STDIN_FILENO; /\* the first command reads from stdin *\/ */
  /* for ( ; i < (nr_commands-1); ++i) { */
  /*   int fd[2]; /\* in/out pipe ends *\/ */
  /*   pid_t pid; /\* child's pid *\/ */
  /*   if (pipe(fd) == -1) { */
  /*     perror("noo"); */
  /*     exit(1); */
  /*   } */
  /*   else if ((pid = fork()) == -1) { */
  /*     /\* report_error_and_exit("fork"); *\/ */
  /*     perror("no fork!"); */
  /*     exit(1); */
  /*   } */
  /*   else if (pid == 0) { /\* run command[i] in the child process *\/ */
  /*     Close(fd[0]); /\* close unused read end of the pipe *\/ */
  /*     run((char* const*)command[i], in, fd[1]); /\* $ command < in > fd[1] *\/ */
  /*   } */
  /*   else { /\* parent *\/ */
  /*     assert (pid > 0); */
  /*     Close(fd[WRITE]); /\* close unused write end of the pipe *\/ */
  /*     Close(in);    /\* close unused read end of the previous pipe *\/ */
  /*     in = fd[READ]; /\* the next command reads from here *\/ */
  /*   } */
  /* } */
  // Run the last process (end of the pipe)
  pid_t pid;
  int fd[2];
  pipe(fd);

  if ((pid = fork()) == 0){
    close(fd[READ]);
    run((char* const*)commands[nr_commands-1], in, STDOUT_FILENO); /* $ command < in */
  }
  int status;
  while ((pid = wait(&status)) != -1) {
    fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
  }
  // Reestablish stdin & stdout
  dup2(tmpin, 0);
  dup2(tmpout,1);
  return 0;
}


void run_terminal () {

  char*** commands;

  /* run all commands but the last */
  do {
    printf(">> ");
    char* input = get_user_input();
    if (input == NULL) { // return
      continue;
    } else {
      commands = parse_input(input);
      printf("arraylen: %d\n", arraylen(commands));
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
