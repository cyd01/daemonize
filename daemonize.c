// gcc -o daemonize -static daemonize.c
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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
    } else if (pid != 0) /* child A */
        exit(0);

    /* child B (grandchild) */
    /* Get parameters */
    if( getenv("DAEMONIZE_SLEEP")!=NULL ) {
        int wait_time = -1 ;
        if( sscanf(getenv("DAEMONIZE_SLEEP"),"%d",&wait_time)>0 ) {
            if( wait_time>=0 ) {
                sleep(wait_time);
            }
        }
    }
    if( getenv("DAEMONIZE_TIME")!=NULL ) {
        int wait_hour = -1 ;
        int wait_min = -1 ;
        if( sscanf(getenv("DAEMONIZE_TIME"),"%d:%d",&wait_hour,&wait_min)>0 ) {
            if( (wait_hour>=0) && (wait_hour<24) && (wait_min>=0) && (wait_min<60) ) {
                struct tm * tm;
                time_t t = 0 ;
                while(1) {
                    t=time(NULL);
                    if( (tm=localtime(&t))!=NULL) {
                        if( (tm->tm_hour==wait_hour) && (tm->tm_min==wait_min) ) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
    // do main command
    execvp(cmd[0],cmd);
}

int main(int argc, char **argv, char **arge) {
    if( argc==1 ) {
        fprintf(stderr, "Usage: %s Command [Options...]\n",basename(argv[0])) ;
        fprintf(stderr, "\n");
        fprintf(stderr, "Environment variables:\n");
        fprintf(stderr, "    DAEMONIZE_TIME=HH:MM wait for a specific time before starting command\n");
        fprintf(stderr, "    DAEMONIZE_SLEEP=SS wait for SS seconds before starting command\n");
    } else {
        daemonize( (char * const *)&argv[1] ) ;
    }
    return 0 ;
}
