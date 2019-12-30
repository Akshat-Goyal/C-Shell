#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "shell.h"


    int jobs(char **cmd, int num){                                  // prints all currently existing jobs
        char *path = (char*) malloc(sizeof(char)*(PATH_MAX+1));
        char *status = (char*) malloc(sizeof(char)*128); 
        size_t size = 0;
        char *buf = NULL;
        for(int i=0; i<running_proc_size; i++){
            sprintf(path, "/proc/%d/status", running[i].pid);
            FILE* file = fopen(path, "r");
            if(file == NULL){
                continue;
            }
            size = 0;
            buf = NULL;
            while(getline(&buf, &size, file) != -1){
                if(strncmp("State", buf, 5) == 0){
                    trim(buf+6, status);
                    break;
                }
            }
            free(buf);
            fclose(file);
            printf("[%d]\t%s\t%s\t[%d]\n", i+1, status, running[i].pname, running[i].pid);
        }
        free(path);
        free(status);
        return 1;
    }