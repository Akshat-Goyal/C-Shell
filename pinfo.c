#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "shell.h"

	int pinfo(char** cmd, int num, char* shell_path){        // prints the process related info of your shell program

		char *id = (char*) malloc(32*sizeof(char));
		if(num == 1){
			sprintf(id, "%d", getpid());
		}
		else{
			strcpy(id, cmd[1]);
		}
		char *path = (char*) malloc(64*sizeof(char));
		strcpy(path, "/proc/");
		strcat(path, id);
		strcat(path, "/stat");

		int size;
		size_t len =0;
		char *buf = NULL;
		char **parse = NULL;
		FILE *file;
		file = fopen(path, "r");

		if(file != NULL) {
			while(getline(&buf, &len, file) != -1){
				parse = split_cmd(buf, &size,  " \t\r\f\v\a\b\n");
				printf("pid -- %s\n", parse[0]);
				printf("Process Status -- %s\n", parse[2]);
				printf("memory -- %s {Virtual Memory}\n", parse[22]);
			}
			free(buf);
		}
		else{
			perror("The process you requested does not exist");
			free(path);free(id);
			return 1;
		}

		strcpy(path, "/proc/");
		strcat(path, id);
		strcat(path, "/exe");

		char buffer[PATH_MAX + 1] = "\0";
		if(readlink(path, buffer, PATH_MAX) == -1){
			perror("The process you requested does not exist");
			fclose(file);free(path);free(id);
			return 1;
		}

		int shell_path_len = strlen(shell_path);
		if(strncmp(shell_path, buffer, shell_path_len) == 0 && (strlen(buffer) == shell_path_len || buffer[shell_path_len] == '/')){
			printf("Executable path - ~%s\n", &buffer[shell_path_len]);
		}
		else{
			printf("Executable path - %s\n", buffer);
		}

		fclose(file);free(id);free(path);
		return 1;
	}