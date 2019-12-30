#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"


int max_proc = 1024, running_proc_size = 0;
struct Proc running[1024];

	int execute_cmd(char*buf, char* shell_path){		// executes command

		int flag = 0, quit_status = 1;
		for(int i=0; buf[i]!='\0' ; i++){
			if(buf[i] == '&'){
				flag = 1;
				buf[i] = '\0';
				break;
			}
		}

		char **cmd;
		int num;										
		cmd = split_cmd(buf, &num, " \t\r\f\v\a\b\n");		// splits command to remove extra spaces, tabs
		if(cmd[0] == NULL){
			return 1;
		}
		if(strcmp(cmd[0], "cd") == 0){
			quit_status = cd(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "exit") == 0 || strcmp(cmd[0], "quit") == 0){
			quit_status = quit();
		}
		else if(strcmp(cmd[0], "pwd") == 0){
			quit_status = pwd();
		}
		else if(strcmp(cmd[0], "ls") == 0){
			quit_status = ls(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "pinfo") == 0){
			quit_status = pinfo(cmd, num, shell_path);
		}
		else if(strcmp(cmd[0], "echo") == 0){
			quit_status = ex_echo(cmd, num);
		}
		else if(strlen(cmd[0]) >= 3 && cmd[0][0] == 27 && cmd[0][1] == 91 && (cmd[0][2] == 65 || cmd[0][2] == 66)){
			quit_status = recall_command(cmd, num);
		}
		else if(strcmp(cmd[0], "setenv") == 0){
			quit_status = ex_setenv(cmd, num);
		}
		else if(strcmp(cmd[0], "unsetenv") == 0){
			quit_status = ex_unsetenv(cmd, num);
		}
		else if(strcmp(cmd[0], "jobs") == 0){
			quit_status = jobs(cmd, num);
		}
		else if(strcmp(cmd[0], "kjob") == 0){
			quit_status = kjob(cmd, num);
		}
		else if(strcmp(cmd[0], "overkill") == 0){
			quit_status = overkill();
		}
		else if(strcmp(cmd[0], "fg") == 0){
			quit_status = fg(cmd, num);
		}
		else if(strcmp(cmd[0], "bg") == 0){
			quit_status = bg(cmd, num);
		}
		else if(strcmp(cmd[0], "history") == 0){
			quit_status = history(cmd, num);
		}
		else if(strcmp(cmd[0], "cronjob") == 0){
			if(running_proc_size == max_proc){
				fprintf(stderr, "Could not execute command: number of processes in background has reached to max limit\n");
				return 1;
			}
			int pid = fork();
			if(pid == -1)
				perror("Error:Could not fork");
			else if(pid == 0){
				cronjob(cmd, num);
				exit(0);
			}
			else{
				kill(pid, 20);
				kill(pid, SIGCONT);
				int temp_len = num;
				for(int i=0; i<num; i++){
					temp_len += strlen(cmd[i]);
				}				
				char* temp = (char*) malloc(temp_len*sizeof(char));
				strcpy(temp, cmd[0]);					
				for(int i=1; i<num; i++){
					strcat(temp, " ");
					strcat(temp, cmd[i]);
				}
				running[running_proc_size].pname = temp;				// list of background process
				running[running_proc_size++].pid = pid;
			}
			return 1;	
		}
		else{
			int pid = fork();								// creates child process
			if(pid == -1){
				perror("Error Forking");
			}
			else if(pid == 0){
				if(flag && running_proc_size == max_proc){
					fprintf(stderr, "Could not execute command: number of processes in background has reached to max limit\n");
					exit(0);
				}
				
				if(flag == 1)
					setpgid(0,0);									// changes gpid to process id
				if(strcmp(cmd[0], "nightswatch") == 0){
					if(!nightswatch(cmd, num)){
						fprintf(stderr, "Could not execute nightswatch\n");
					}
				}
				else if(execvp(cmd[0], cmd) == -1){				// execvp executes system commands 
					perror("Could not execute command");
				}
				exit(0);
			}
			else if(pid > 0){
				if(flag != 1){
					if(child_pid == -1) child_pid = pid;
					int status, shell_pid = getpid();
					waitpid(pid, &status, WUNTRACED);		// waits for child process
					if(WIFSTOPPED(status)){					// if child was stopped, add it in background process list
						if(running_proc_size == max_proc){
							kill(pid, SIGINT);
							fprintf(stderr, "Could not execute command: number of processes in background has reached to max limit\n");
							exit(0);
						}
						int temp_len = num;
						for(int i=0; i<num; i++){
							temp_len += strlen(cmd[i]);
						}				
						char* temp = (char*) malloc(temp_len*sizeof(char));
						strcpy(temp, cmd[0]);					
						for(int i=1; i<num; i++){
							strcat(temp, " ");
							strcat(temp, cmd[i]);
						}
						running[running_proc_size].pname = temp;				// list of background process
						running[running_proc_size++].pid = pid;
					}
				}	
				else{
					fprintf(stderr, "%d\n", pid);
					int temp_len = num;
					for(int i=0; i<num; i++){
						temp_len += strlen(cmd[i]);
					}				
					char* temp = (char*) malloc(temp_len*sizeof(char));
					strcpy(temp, cmd[0]);				
					for(int i=1; i<num; i++){
						strcat(temp, " ");
						strcat(temp, cmd[i]);
					}
					running[running_proc_size].pname = temp;					// stores pid of background process in array
					running[running_proc_size++].pid = pid;
				}
			}
		}
		return quit_status;
	}