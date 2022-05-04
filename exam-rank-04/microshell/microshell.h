#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int has_pipe;
int ret;
int to_stdin;

#endif