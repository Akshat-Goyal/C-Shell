#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "shell.h"

	char** split_cmd(char* cmd, int* num, char delim[]){       // parses the command according to delim string
		int lines_no = 32, cnt = 0;

		char *token;
		char **tokens = (char**) malloc(lines_no * sizeof(char*));
		token = strtok(cmd, delim);
		do{
			tokens[cnt++] = token;

			if(cnt > lines_no){
				lines_no *= 2;
				tokens = (char**) realloc(tokens, lines_no * sizeof(char*));
			}

			token = strtok(NULL, delim);

		} while(token != NULL);
		tokens[cnt] = NULL;
		*num = cnt;
		return tokens;
	}

	void trim(char *from, char *to){							// removes tabs and spaces before and after the string
		int len = strlen(from), i, j;
		for(i=0; i<len; i++){
			if(!isspace(from[i]))
				break;
		}
		for(j=len-1; j>=0; j--){
			if(!isspace(from[j]))
				break;
		}
		strncpy(to, &from[i], j-i+1);
		to[j-i+1] = '\0';
	}