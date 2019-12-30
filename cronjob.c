#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

    int cronjob(char ** cmd, int num){                  // cronjob executes a particular command in fixed time interval for a certain period
        int c_st = 0, c_en = 0, time = -1, period = -1, i=1;
        while(i<(num-1) && strlen(cmd[i]) == 2){
            if(cmd[i][0] == '-' && cmd[i][1] == 't'){
                if(time != -1){
                    time = -1; break;
                }
                time = atoi(cmd[i+1]); i+=2;
            }
            else if(cmd[i][0] == '-' && cmd[i][1] == 'p'){
                if(period != -1){
                    period = -1; break;
                }
                period = atoi(cmd[i+1]); i+=2;
            }
            else if(cmd[i][0] == '-' && cmd[i][1] == 'c'){
                if(c_st!=0){
                    c_st = 0; break;
                }
                if(cmd[i+1][0] == '['){
                    c_st = i+1;
                    for(int j=i+1; j<num; j++){
                        if(cmd[j][strlen(cmd[j])-1] == ']'){
                            c_en = j; i=j+1; break;
                        }
                    }
                }
                else{
                    c_st = c_en = i+1; i+=2;
                }
            }
            else{
                break;
            }
        }
        if(i != num || c_st == 0 || c_en == 0 || period == -1 || time == -1){
            fprintf(stderr, "Error: invalid format\n");
            return 1;
        }
        int command_len = c_en - c_st + 1;
        for(int j=c_st; j<=c_en; j++){
            command_len += strlen(cmd[j]);
        }
        char *command = (char*) malloc(sizeof(char)*command_len);
        if(cmd[c_st][0] == '[' && strlen(cmd[c_st]) == 1){
            command[0] = '\0';
        }
        else if(cmd[c_st][0] == '['){
            strcpy(command, &cmd[c_st][1]);
        }
        else
            strcpy(command, cmd[c_st]);
        for(int j=c_st+1; j<=c_en; j++){
            strcat(command, " ");
            strcat(command, cmd[j]);
        }
        if(cmd[c_en][strlen(cmd[c_en])-1] == ']')
            command[strlen(command)-1] = '\0';
        else
            command[strlen(command)] = '\0';
        if(command[0] == '\0'){
            fprintf(stderr, "Error: invalid format\n");
            return 1;
        }
        i = time;
        while(i <= period){
            sleep(time);
            printf("\n");
            execute_pipe(command);                  // executes command
            i += time;
        }
        free(command);
        return 1;
    }