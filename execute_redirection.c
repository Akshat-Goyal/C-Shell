#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include "shell.h"


	int execute_redirection(char *cmd){
		int stdin_fd = dup(0);
		int stdout_fd = dup(1);
		char *s = (char*) malloc(sizeof(char)*(PATH_MAX+1));
		char *write_file = (char*) malloc(sizeof(char)*(PATH_MAX+1));
		char *read_file = (char*) malloc(sizeof(char)*(PATH_MAX+1));
		int cmd_len = strlen(cmd), read_fd = -1, write_fd = -1;
		read_file[0] = '\0';
		write_file[0] = '\0';

		for(int i=cmd_len-1; i>=0; i--){							// this loop checks '>'
			if(cmd[i] == '>'){
				int j = i+1;
				while(j<cmd_len && cmd[j]!='<') j++;
				strncpy(s, &cmd[i+1], j-i-1);
				s[j-i-1] = '\0';
				trim(s, write_file);
				if(i>0 && cmd[i-1] == '>')
					write_fd = open(write_file, O_WRONLY|O_CREAT|O_APPEND, 0644);
				else
					write_fd = open(write_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
				if(write_fd == -1){
					perror(write_file);
					free(write_file);free(read_file);free(s);
					return 1;
				}
				dup2(write_fd, 1);
				break;
			}
		}
		for(int i=cmd_len-1; i>=0; i--){							// this loop checks '<'
			if(cmd[i] == '<'){
				int j = i+1;
				while(j<cmd_len && cmd[j]!='>') j++;
				strncpy(s, &cmd[i+1], j-i-1);
				s[j-i-1] = '\0';
				trim(s, read_file);
				struct stat st;
				if(stat(read_file, &st) == -1){
					perror(read_file);
					free(write_file);free(read_file);free(s);
					return 1;
				}
				if(!S_ISREG(st.st_mode)){
					fprintf(stderr, "Error: %s is not a regular file\n", read_file);
					free(write_file);free(read_file);free(s);
					return 1;
				}
				read_fd = open(read_file, O_RDONLY);
				if(read_fd == -1){
					perror(read_file);
					free(write_file);free(read_file);free(s);
					return 1;
				}
				dup2(read_fd, 0);
				break;
			}
		}
		int i=0;
		while(i<cmd_len && cmd[i]!='>' && cmd[i]!='<') i++;
		strncpy(s, cmd, i);
		s[i] = '\0';
		int status = execute_cmd(s, shell_path);					// executes command
		dup2(stdin_fd, 0);
		dup2(stdout_fd, 1);
		if(read_fd != -1) close(read_fd);
		if(write_fd != -1) close(write_fd);
		free(write_file);
		free(read_file);
		free(s);
		return status;
	}