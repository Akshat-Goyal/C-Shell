#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"


	int kjob(char **cmd, int num){								// kills the given job with given signal
		if(num!=3){
			fprintf(stderr, "Error: invalid format\n");
			return 1;
		}
		int job_no = atoi(cmd[1]), sig_no = atoi(cmd[2]);
		if(job_no > running_proc_size || job_no < 1){
			fprintf(stderr, "Error: invalid job id\n");
			return 1;
		}
		kill(running[job_no-1].pid, sig_no);
		return 1;
	}

	int overkill(){												// kills all background jobs with SIGINT
		for(int i=0; i<running_proc_size; i++){
			kill(running[i].pid, 9);
		}
		return 1;
	}