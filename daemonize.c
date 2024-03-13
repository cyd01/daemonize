// gcc -o daemonize -static daemonize.c
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void daemonize (char * const *cmd) {
    int pid ;
    if( cmd==NULL ){
        fprintf(stderr,"No command found");
        exit(EXIT_FAILURE);
    }
    if( cmd[0]==NULL ){
        fprintf(stderr,"No command found");
        exit(EXIT_FAILURE);
    }
    if( strlen(cmd[0])==0)
        exit(0);
    /*
     * Some missing stuff here
     */

    if ((pid = fork()) < 0) { 
        fprintf(stderr,"fork fail\n" ) ;
        exit(EXIT_FAILURE) ;
    } else if (pid != 0) /* parent */
        exit(0);

    /* child A */
    /* create session and set process group ID */
    setsid();

    if ((pid = fork()) < 0){
        fprintf(stderr,"fork fail\n" ) ;
        exit(EXIT_FAILURE) ;
    } else if (pid != 0) /* child A * /
        exit(0);

    /* child B (grandchild) */
    // do main command
    execvp(cmd[0],cmd);
}

int main(int argc, char **argv, char **arge) {
    if( argc==1 ) {
        fprintf(stderr, "Usage: %s Command [Options...]\n",basename(argv[0])) ;
    } else {
        daemonize( (char * const *)&argv[1] ) ;
    }
    return 0 ;
}
