/* Christopher Sickler */
/* cbs2468 */
/* imports go here */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shell.h"
#include "vector.h"
#include "token.h"

pid_t pid;
Shell *shell;

void prompt(Shell *sh) {
	printf("%s", sh->prompt);
}

void readLine(Shell *sh) {
	// printf("readLine\n");
	size_t pos = 0;
	char* buf = sh->line;
	char c;
	while(1){
		c =getchar();

		if(c == '\n'){
			buf[pos] = '\0';
			return;
		} else if (c == EOF) {
			exit(0);
		} else if (c == "\027") {

        } else {
			buf[pos] = c;
		}
		pos++;
	}
}

int replaceFile(int toClose, const char * filepath, int flags, mode_t mode) {
    int status = 0;
    status = close(toClose);
    if(status != 0)
        return 0;
    int fd = open(filepath, flags, mode);
    if(fd < 0){
        return 0;
    }

    return 1;
}

#define RWURGRO ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )
int parseCommand(Command *cmd, char **argList[]) {

    Token *tok = cmd->tok;
    const char * token;
    Vector *args = VectorInit(1);
    while(TokenHasTokens(tok)) {
        token = TokenNext(tok);
        if(!strcmp(token, ">")) {
            if(!TokenHasTokens(tok))
                return 0;
            replaceFile(STDOUT_FILENO, TokenNext(tok), O_WRONLY | O_CREAT, RWURGRO);
        } else if (!strcmp(token, "2>")) {
            if(!TokenHasTokens(tok))
                return 0;
            replaceFile(STDERR_FILENO, TokenNext(tok), O_WRONLY | O_CREAT, RWURGRO);
        } else if (!strcmp( token, "<")) {
            if(!TokenHasTokens(tok))
                return 0;
            replaceFile(STDIN_FILENO, TokenNext(tok), O_RDONLY, RWURGRO);
        } else {
            VectorPush(args, token);
        }
    }

    int argc = VectorLength(args);
    char ** argv = malloc(sizeof(char *) * (argc + 1) );
    for( int i = 0; i < argc; i++ )
    {
        argv[i] = VectorGet(args, i);
    }
    argv[argc] = NULL;

    VectorDelete(args);

    *argList = argv;
    return 1;
}

void setupPipes(Command *cmd) {
    Pipe *inPipe = cmd->inPipe;
    Pipe *outPipe = cmd->outPipe;
    if(inPipe) {
        close(inPipe->fd[1]);
        dup2(inPipe->fd[0], STDIN_FILENO);
    }
    if(outPipe) {
        close(outPipe->fd[0]);
        dup2(outPipe->fd[1], STDOUT_FILENO);
    }
}

void cleanPipes(Pipe *pipes, int numPipes) {
    for( int i = 0; i < numPipes; i++ ) {
        close(pipes[i].fd[0]);
        close(pipes[i].fd[1]);
    }
}

void exec(Command *cmd, Pipe *pipes, int numPipes) {
	printf("Exec\n");
	char **argList;
    if(!parseCommand( cmd, &argList )) {
        printf("Child failed to setup exec!\n");
        return 0;
    }
    if(pipes) {
        setupPipes( cmd );
        cleanPipes( pipes, numPipes );
    }
    execvp(argList[0], argList);
    free(argList);
    exit( 0 );
}

pid_t forkexec(Command *cmd, Pipe *pipes, int numPipes) {
	pid_t cpid = fork();
	if (cpid == 0) {
		exec(cmd,pipes,numPipes);
	}
	return cpid;
}

pid_t forkexec_grp(Command **cmds, int numCmds) {
	pid_t cpid = fork();
	if (cpid == 0) {
		setpgid(0,0);
		Pipe *pipes = malloc(sizeof(Pipe) * (numCmds-1));
		for (int i = 0; i < numCmds; i++) {
			pipe(pipes[i].fd);
		}

		cmds[0]->inPipe = NULL;
        cmds[0]->outPipe = &pipes[0];
        for(int i = 1; i < numCmds-1; i++) {
            cmds[i]->inPipe = &pipes[i-1];
            cmds[i]->outPipe = &pipes[i];
        }
        cmds[numCmds-1]->inPipe = &pipes[numCmds-2];
        cmds[numCmds-1]->outPipe = NULL;

		for (int i = 0; i < numCmds; i++) {
			forkexec(cmds[i], pipes, numCmds-1);
		}

		exec(cmds[0], pipes, numCmds-1);
	}
	return cpid;
}

void addProcess(Shell *sh, Process * pr) {
	VectorPush(sh->procTable, pr);
}

void remProcess(Shell * sh, Process * pr) {
	VectorRemove(sh->procTable, pr);
}

void pushSusp(Shell * sh, Process * pr) {
	VectorPush(sh->suspStack, pr);
}

void popSusp(Shell * sh, Process **prHandle) {
	*prHandle = VectorPop(sh->suspStack);
}

void waitActive(Shell *sh);

int checkJobs( Shell * sh ) {
    if( !strcmp(sh->line, "fg" ) ) {
        popSusp( sh, &sh->active );
        if( sh->active ) {
            sh->active->state = fg;
            printf("fg %d\n", sh->active->pid);
            kill( sh->active->pid, SIGCONT );
            waitActive(sh);
        }
        else {
            printf("No suspended jobs\n");
        }
        return 1;
    }
    else if ( !strcmp(sh->line, "bg") ) {
        return 1;
    }
    return 0;
}

void parseLine (Shell *sh) {
	printf("parseLine\n");
	if (checkJobs(sh)) {
		return;
	}
	printf("xDD\n");
	Token *tok = TokenInit(sh->line, "|");
	int numCmds = TokenNumTokens(tok);
	printf("%d\n",numCmds);
	State state = strchr(sh->line,'&')? bg: fg;
	pid_t cpid;
	if (numCmds == 1) {
		Token *cmdtok = TokenInit(TokenNext(tok),"&");
		Command cmd;
		CommandConst(&cmd, TokenNext(cmdtok));
		cpid = forkexec(&cmd,NULL,0);
		Process *child = ProcessInit(sh->line,cpid,state);
		addProcess(sh,child);
		printf("child should be made\n");
		if (state == fg) {
			sh->active = child;
			waitpid(child->pid);
		} else if (state == bg) {
			printf("bg %d\n",child->pid);
		}
		CommandDestr(&cmd);
		TokenDelete(cmdtok);
	} else if (numCmds > 1) {
		Command **cmds = malloc(sizeof(Command *) * numCmds);
		for (int i = 0; i < numCmds; i++) {
			cmds[i] = CommandInit(TokenNext(tok));
		}

		cpid = forkexec_grp(cmds,numCmds);
		waitpid(-cpid);

		for (int i = 0; i < numCmds; i++) {
			free(cmds[i]);
		}
		free(cmds);
	}
	TokenDelete(tok);
}

void sigintHandler(int signo) {
	prompt(shell);
	if (!shell->active) {
		return;
	}
	kill(shell->active->pid,SIGINT);
}

void sigtstpHandler(int signo) {
    prompt(shell);
    if(!shell->active) {
        return;
    }
    kill( shell->active->pid, SIGTSTP );
}

void sigchldHandler(int signo) {
	prompt(shell);
	if (!shell->active) {
		return;
	}
	kill(shell->active->pid,SIGCHLD);
}

void waitActive(Shell *sh) {
	// printf("waitActive\n");
	int wstatus;
	int wpid;
	if (sh->active) {
		printf("MEOW");
		dprintf("Waiting on %d \"%s\"\n", sh->active->pid, sh->active->command);
        wpid = waitpid( sh->active->pid, &wstatus, WUNTRACED | WCONTINUED );
		if(WIFEXITED(wstatus)) {
            dprintf("%d exited\n", wpid);
            remProcess( sh, sh->active );
            sh->active = NULL;
        }
        else if (WIFSTOPPED(wstatus)) {
            dprintf("%d stopped\n", wpid);
            sh->active->state = sp;              // suspend it
            pushSusp(sh, sh->active);  // push it onto the susp stack
            sh->active = NULL;
        }
        else if (WIFCONTINUED(wstatus)) {
            dprintf("%d continued\n", wpid);
        }
	}
}

int main( int argc, char *argv[] ) {
	Shell *sh = shellInit();
	Shell_setPrompt(sh, "# ");

	shell = sh;
	pid = getpid();

	signal(SIGTSTP, sigtstpHandler);
	signal(SIGINT, sigintHandler);
	signal(SIGCHLD, sigchldHandler);

	while(1) {
		prompt(sh);
		readLine(sh);
		parseLine(sh);
		waitActive(sh);
	}
}