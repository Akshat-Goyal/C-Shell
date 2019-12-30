#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "shell.h"

char *shell_path;
int child_pid = -1;

	int main(int argc, char **argv){
		signal(SIGINT, signal_handler);				// ctrl^c is ignored
		signal(SIGTSTP, signal_handler);			// ctrl^z is ignored
		signal(SIGCHLD, signal_handler);
		signal(SIGQUIT, signal_handler);

		int status = 1;
		size_t buf_size = 0;
		char *buf = (char*) malloc(buf_size);
		char **cmds;
		int cmd_no;

		shell_path = get_shell_path(argv[0]);		// return shell's absolute path
		load_history();								// previous history is loaded
		do{
			int pid, pid_status;					// waitpid provides pid of child process which changed its state
			while((pid = waitpid(-1, &pid_status, WNOHANG | WUNTRACED)) > 0){
				if(WIFEXITED(pid_status)){			// WIFEXITED returns 1 if process exited normally
					for(int i=0; i<running_proc_size; i++){
						if(running[i].pid == pid){
							fprintf(stderr, "%s with pid %d exited normally.\n", running[i].pname, pid);
							free(running[i].pname);
							running_proc_size--;
							while(i<running_proc_size){
								running[i] = running[i+1];
								i++;
							}
							break;
						}
					}
				}
				else if(WIFSIGNALED(pid_status)){		// WIFEXITED returns 1 if process exited abnormally
					for(int i=0; i<running_proc_size; i++){
						if(running[i].pid == pid){
							fprintf(stderr, "%s with pid %d terminated by a signal.\n", running[i].pname, pid);
							free(running[i].pname);
							running_proc_size--;
							while(i<running_proc_size){
								running[i] = running[i+1];
								i++;
							}
							break;
						}
					}
				}
			}
			display(shell_path);						// displays username@sysname:filepath
			buf = NULL;buf_size = 0;
			if(getline(&buf, &buf_size, stdin) == -1){
				printf("\33[2K\r");
				continue;
			}
			if(buf[0] != '\n' && buf[0] != ' ' && buf[0]!=27){
				insert_cmd(buf);						// command is added to history
			}
			cmds = split_cmd(buf, &cmd_no, ";");		// ';' separated commands 
			for(int i=0; i<cmd_no; i++){
				child_pid = -1;
				if (*cmds[i] == '\n') {
					continue;
				}
				status = execute_pipe(cmds[i]);			// executes the command	

				if(status == 0)
					break;
			}
			free(buf);
		} while(status != 0);
		free(shell_path);
		return 0;
	}