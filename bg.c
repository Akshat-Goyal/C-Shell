#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"


    int bg(char **cmd, int num){                            // continues background process
        if(num != 2){
            fprintf(stderr, "Error: invalid format\n");
            return 1;
        }
        int job_no = atoi(cmd[1]);
        if(job_no > running_proc_size || job_no < 1){
            fprintf(stderr, "Error: invalid job id\n");    
        }
        else{
            kill(running[job_no-1].pid, SIGCONT);            // sigcont to continue process
        }
        return 1;
    }