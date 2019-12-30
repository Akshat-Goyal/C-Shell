#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


	int fg(char **cmd, int num){									// brings process in foreground
		if(num != 2){
			fprintf(stderr, "Error: invalid format\n");
			return 1;
		}
		int job_no = atoi(cmd[1]), status;
		if(job_no > running_proc_size || job_no < 1){
            fprintf(stderr, "Error: invalid job id\n");    
        }
        else{
			int shell_pid = getpid();
			if(strncmp(running[job_no-1].pname, "cronjob", 7) == 0){
				fprintf(stderr, "Error: Cronjob only runs in background\n");
				return 1;
			}
			printf("%s\n", running[job_no-1].pname);
			if(child_pid == -1) child_pid = running[job_no-1].pid;
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
			tcsetpgrp(0, getpgid(running[job_no-1].pid));
			kill(running[job_no-1].pid, SIGCONT);					// sigcont to continue process
			waitpid(running[job_no-1].pid, &status, WUNTRACED);
			tcsetpgrp(0, shell_pid);
			signal(SIGTTOU, SIG_DFL);
			signal(SIGTTIN, SIG_DFL);
			if(!WIFSTOPPED(status)){
				free(running[job_no-1].pname);
				running_proc_size--;
				for(int i=job_no-1; i<running_proc_size; i++){
					running[i] = running[i+1];
				}
			}
        }
		return 1;
	}