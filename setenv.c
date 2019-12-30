#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

	int ex_setenv(char** cmd, int num){								// sets environment variable 
		if(num <= 1){
			fprintf(stderr, "Error: invalid format\n");
			return 1;
		}
		if(num == 2){
			if(setenv(cmd[1], "", 1) == -1)
				perror("Could not execute setenv\n");
		}
		else if(cmd[2][0] == '[' && cmd[num-1][strlen(cmd[num-1])-1] == ']'){
			int len=0;
			for(int i=2;i < num; i++){
				len+=strlen(cmd[i]);
			}
			char *val = (char*) malloc(sizeof(char)*len);
			if(strlen(cmd[2]) == 1)
				val[0] = '\0';
			else
				strcpy(val, &cmd[2][1]);
			for(int i=3; i < num; i++){
				strcat(val, cmd[i]);
			}
			val[strlen(val)-1] = '\0';
			if(setenv(cmd[1], val, 1) == -1)
				perror("Could not execute setenv\n");
			free(val);
		}
		else{
			fprintf(stderr, "Error: invalid format\n");
		}
		return  1;
	}

	int ex_unsetenv(char **cmd, int num){							// unsets environment variable 
		if(num < 2){
			fprintf(stderr, "Error: invalid format\n");
			return 1;
		}
		if(unsetenv(cmd[1]) == -1){
			perror("Could not execute unsetenv\n");
		}
		return 1;
	}
