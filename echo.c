#include <stdio.h>
#include "shell.h"

	int ex_echo(char **cmd, int num){			// echoes the command given
		for(int i=1; i<num-1; i++){
			printf("%s ", cmd[i]);
		}
		printf("%s", cmd[num-1]);
		printf("\n");
		return 1;
	}