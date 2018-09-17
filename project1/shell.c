#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "token.h"
#include "vector.h"

Shell * shellInit(void) {
    Shell *sh = malloc(sizeof(Shell));

    ShellConst(sh);

    return sh;
}

void ShellConst(Shell *sh) {
    sh->prompt = malloc(PROMPT_SIZE * sizeof(char) + 1);
    sh->prompt[0] = '\0';

    sh->line = malloc(LINE_SIZE * sizeof(char) + 1);
    sh->line[0] = '\0';

    sh->active = NULL;
    sh->procTable = VectorNewReg(1, &ProcDelete);
    sh->suspStack = VectorInit(1);
}

void ShellDestr(Shell *sh) {
    free(sh->prompt);
    sh->prompt = NULL;

    free(sh->line);
    sh->line = NULL;

    VectorDeleteFull(sh->procTable);
    sh->procTable = NULL;

    VectorDelete(sh->suspStack);
    sh->suspStack = NULL;

    sh->active = NULL;
}

void Shell_setPrompt(Shell *sh, const char *prompt) {
    strcpy(sh->prompt, prompt);
}

void ProcessConst(Process *pr, const char *line, pid_t pid, State state) {
    pr->pid = pid;
    pr->state = state;
    pr->command = strdup(line);
}

void ProcessDestr(Process *pr) {
    free(pr->command);
}

void ProcDelete(Process *pr) {
    ProcessDestr(pr);
    free(pr);
}

Command *CommandInit(const char *line) {
    Command *cmd = malloc(sizeof(Command));
    CommandConst(cmd, line);
    return cmd;
}

void CommandConst(Command *cmd, const char *line) {
    cmd->tok = TokenInit(line, " ");
    cmd->inPipe = this->outPipe = NULL;
}

void CommandDestr(Command *cmd) {

}
