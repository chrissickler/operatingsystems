#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdlib.h>


#include "token.h"
#include "vector.h"

#define CWD_SIZE 1000
#define PROMPT_SIZE 1000
#define LINE_SIZE 2000

typedef enum {
    fg,
    bg,
    dn
} State;

typedef struct {
    pid_t pid;
    State state;
    char *command;
} Process;

typedef struct {
    int fd[2];
} Pipe;

typedef struct {
    Token *tok;
    Pipe *inPipe;
    Pipe *outPipe;
} Command;

typedef struct {
    char *cwd;
    char *prompt;
    char *line;

    Vector *procTable;
    Vector *suspStack;
    Process *active;
} Shell;

Shell * shellInit(void);
void ShellConst(Shell *sh);
void ShellDestr(Shell *sh);

Command *CommandInit(const char *line);
void CommandConst(Command *cmd, const char *line);
void CommandDestr(Command *cmd);

void ProcessConst(Process *pr, const char *line, pid_t pid, State state);
void ProcessDestr(Process *pr);
Process * ProcessInit(const char *line, pid_t, State state);

void ProcDelete (Process *pr);

void Shell_setPrompt(Shell *sh, const char *prompt);

#endif
