/**##############################################################
## MODULE: psh.c
## VERSION: 1.2 
## SINCE: 2013-09-11
## AUTHOR: 
##     Jimmy Lin (xl5224, loginID: jimmylin) - JimmyLin@utexas.edu  
##     Bochao Zhan (bz2892, loginID: bzchao)- bzhan927@gmail.com
## DESCRIPTION: 
##     psh - A prototype tiny shell program with job control
##
#################################################################
## Edited by MacVim
## Class Info auto-generated by Snippet 
################################################################*/

/* Used Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "util.h"

/* Global variables */
int verbose = 0;            /* if true, print additional output */

extern char **environ;      /* defined in libc */
static char prompt[] = "psh> ";    /* command line prompt (DO NOT CHANGE) */
int MAX_NUM_ARGS = 10;
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
void rmNewLine(char *str);
void rmWhiteSpaces(char *str);
int builtin_cmd(char **argv);

/* Here are helper routines that we've provided for you */
void usage(void);
void sigquit_handler(int sig);



/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
        }
    }


    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  
/*
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return. 
*/
void eval(char *cmdline) 
{
    char * delimiter = " \n";  // separator between command and arguments
    char * command = strtok(cmdline, delimiter);  // record the command
    if (!builtin_cmd(&command)) {  // check for built-in command
        pid_t child = fork();  // create child process
        if (child == 0) {  // child process
            // Initialiization of execution information
            char * args[MAX_NUM_ARGS];
            // Set null to all element of args
            int j;  // index variable 
            for (j = 0; j < MAX_NUM_ARGS; j ++) 
                args[j] = NULL;

            // configure the arguments
            int i = 0;  // index variable
            args[i++] = command;  // recognition of the command
            // recognition ofr the following arguments
            char * pch;
            while ((pch = strtok(NULL, delimiter)) != NULL) {
                // check for the upper bound of argument number
                if (!(i < MAX_NUM_ARGS)) {
                    fprintf(stderr, "Too many arguments..");
                    break;
                }
                args[i++] = pch;  // assignment for args
            }
            // execution, here we
            if (execvp(args[0], args) == -1) 
                printf("Wrong usage of PSH.\n");
            exit(EXIT_SUCCESS);
        } else {
            int status = 0;
            // wait the job to finish
            waitpid(child, &status, 0);
        }
    }
    return;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately. 
 * Return 1 if a builtin command was executed; return 0
 * if the argument passed in is *not* a builtin command.
 */
int builtin_cmd(char **argv) 
{ 
    // The quit command terminates the shell.
    if (strcmp(*argv, "quit") == 0) {
        exit(1); // user-specified quit 
    }

    return 0;     /* not a builtin command */
}



/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



