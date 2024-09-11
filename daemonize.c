// gcc -o daemonize -static daemonize.c
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* getopt stuff */
extern char * optarg;
extern int optind, opterr;

/* Waiting time (in seconds) before starting */
int debug_flag=0;
int sleeping_time=0 ;
char *waiting_time=NULL;
static void usage(char* program, int exitcode) ;

void daemonize (char * const *cmd) {
    if( debug_flag ) {
        fprintf(stderr,"DEBUG: start daemon command <%s",cmd[0]);
        int i=1;
        while( cmd[i]!=NULL ) {
            fprintf(stderr," %s",cmd[i]);
            i++;
        }
        fprintf(stderr,">\n");
    }
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
    if( sleeping_time>=0 ) { /* waiting for sleeping_time seconds */
        sleep(sleeping_time);
    }

    if( waiting_time!=NULL ) { /* waiting for time is waiting_time */
        int wait_hour = -1 ;
        int wait_min = -1 ;
        if( sscanf(waiting_time,"%d:%d",&wait_hour,&wait_min)>0 ) {
            if( (wait_hour>=0) && (wait_hour<24) && (wait_min>=0) && (wait_min<60) ) {
                struct tm * tm;
                time_t t = 0 ;
                while(1) {
                    t=time(NULL);
                    if( (tm=localtime(&t))!=NULL) {
                        if( (tm->tm_hour==wait_hour) && (tm->tm_min==wait_min) ) {
                            break;
                        } else {
                            sleep(5);
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
    int opt;
    if( getenv("DAEMONIZE_SLEEP")!=NULL ) {
        int sleep_time = -1 ;
        if( sscanf(getenv("DAEMONIZE_SLEEP"),"%d",&sleep_time)>0 ) {
            if( sleep_time>=0 ) {
                sleeping_time=sleep_time;
            }
        }
    }
    if( getenv("DAEMONIZE_TIME")!=NULL ) {
        waiting_time=(char*)malloc(strlen(getenv("DAEMONIZE_TIME"))+1);
        strcpy(waiting_time,getenv("DAEMONIZE_TIME"));
    }

    while ((opt=getopt(argc, argv, "dhs:t:"))!=EOF) {
      switch (opt) {
        case 'd': debug_flag=1 ; break ;
        case 'h': usage(basename(argv[0]),0) ; break ;
        case 's': 
          int sleep_time = -1 ;
          if( sscanf(optarg,"%d",&sleep_time)>0 ) {
              if( sleep_time>=0 ) {
                  sleeping_time=sleep_time;
                  if( debug_flag ) { fprintf(stderr,"DEBUG: set sleeping time to <%d>\n",sleeping_time); }
              }
          }
          break ;
        case 't': 
          waiting_time=(char*)malloc(strlen(optarg)+1);
          strcpy(waiting_time,optarg);
          if( debug_flag ) { fprintf(stderr,"DEBUG: set waiting time to <%s>\n",waiting_time); }
          break ;
        default: fprintf(stderr,"Wrong parameter\n"); usage(basename(argv[0]),1) ;
      }
    }

    if( argc<=optind ) {
        fprintf(stderr,"Not enought parameters, missing command\n");
        usage(basename(argv[0]),1) ;
    } else {
        daemonize( (char * const *)&argv[optind] ) ;
    }
    return 0 ;
}

static void usage(char *program, int exitcode) {
  fprintf(stderr, 
    "Usage: %s [-options...] Command [Parameters...]\n"
    "  start Command as a daemon\n"
    "\t-s <sec>    sleeping before starting (in seconds)\n"
    "\t-t <HH:MM>  waiting time before starting (format HH:MM)\n"
    "\n"
    "Environment variables:\n"
    "\tDAEMONIZE_TIME=HH:MM  wait for a specific time before starting command\n"
    "\tDAEMONIZE_SLEEP=SS    wait for SS seconds before starting command\n"
    , program
    );
  exit(exitcode);
}
