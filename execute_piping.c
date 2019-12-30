#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


	int execute_pipe(char *cmd){
		cmd = cmd_without_tilde(cmd);								// returns command replacing tilde with shell path
		int pipe_fd[2];
		int cmd_pipe_no = 0;
		char **cmd_pipe = split_cmd(cmd, &cmd_pipe_no, "|");		// '|' separated commands
		int stdin_fd = dup(0);
		int stdout_fd = dup(1);

		if(cmd_pipe_no == 1){
			int status = execute_redirection(cmd_pipe[0]);			// executes command
			free(cmd);
			return status;
		}	
		for(int i=0, pid, status; i<cmd_pipe_no; i++){
			if(i!=0){
				close(pipe_fd[1]);
				stdin_fd = dup(0);
				if(dup2(pipe_fd[0], 0) == -1)
					perror("Could not exeute dup2 ");
				close(pipe_fd[0]);
			}
			if(i!=cmd_pipe_no-1){
				if(pipe(pipe_fd) == -1)
					perror("Could not execute pipe ");
				stdout_fd = dup(1);
				if(dup2(pipe_fd[1], 1) == -1)
					perror("Could not exeute dup2 ");
			}
			pid = fork();
			if(pid == -1){
				dup2(stdin_fd, 0);
				dup2(stdout_fd, 1);
				perror("Could not exeute fork ");
				return 1;
			}
			else if(pid == 0){
				execute_redirection(cmd_pipe[i]);
				exit(0);
			}
			else{
				waitpid(pid, &status, WUNTRACED);		// waits for child process
				dup2(stdin_fd, 0);
				dup2(stdout_fd, 1);
			}
		}
		free(cmd);
		return 1;
	}