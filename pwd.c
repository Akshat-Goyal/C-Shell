#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "shell.h"


	int pwd(){						// prints the present working directory using getcwd
		char cwd[PATH_MAX + 1];
		getcwd(cwd, PATH_MAX + 1);
		printf("%s\n", cwd);
		return 1;
	}