#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "shell.h"


	int cd(char **cmd, int num, char* shell_path){		// changes the directory
		if(num == 1){
			if(chdir(shell_path) == -1){			// chdir changes directory
				perror(shell_path);
			}	
		}
		else if(chdir(cmd[1]) == -1){
			perror(cmd[1]);
		}
		return 1;
	}