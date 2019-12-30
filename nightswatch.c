#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "shell.h"


	int watch_interrupt(int sec){						// prints number of times that particular CPU has been interrupted by the keyboard controller
		char path[] = "/proc/interrupts";
		FILE *file;
		char *buf = NULL, str[64];
		char **cpu;
		size_t size = 0;
		int len = 0;
		clock_t start = clock();
		initscr();
		nodelay(stdscr, TRUE);
		char ch = getch();
		int y = 0, x = 0;
		for(int i=0; i<8; i++){
			sprintf(str, "CPU%d", i);
			mvprintw(y, x, str);
			x += 10;
		}
		y += 1;
		while(1){
			file = fopen(path, "r");
			buf = NULL;
			size = 0;
			for(int i=0; i<3; i++){
				getline(&buf, &size, file);
			}
			cpu = split_cmd(buf, &len, " \t\n");
			x = 0;
			for(int i=0; i<8; i++){
				mvprintw(y, x, cpu[i+1]);
				x += 10;
			}
			fclose(file);
			free(buf);

			start = clock();
			while((clock() - start)/CLOCKS_PER_SEC < sec){
				ch = getch();
				if(ch == 'q'){
					endwin();
					return 1;
				}
			}
			y += 1;
		}
		return 1;
	}

	int watch_dirty(int sec){				// prints the size of the part of file which is dirty
		char path[] = "/proc/meminfo";
		FILE *file;
		char *buf = NULL, str[64];
		size_t size = 0;
		int len = 0;
		clock_t start = clock();
		char **cpu;
		initscr();
		nodelay(stdscr, TRUE);
		char ch = getch();
		int y = 0;
		while(1){
			file = fopen(path, "r");
			buf = NULL;
			size = 0;
			for(int i=0; i<17; i++){
				getline(&buf, &size, file);
			}
			cpu = split_cmd(buf, &len, " \t\n");

			sprintf(str, "%s %s", cpu[0], cpu[1]);
			mvprintw(y, 0, str);
			fclose(file);
			free(buf);
			
			start = clock();
			while((clock() - start)/CLOCKS_PER_SEC < sec){
				ch = getch();
				if(ch == 'q'){
					endwin();
					return 1;
				}
			}
			y += 1;
		}
		return 1;
	}


	int nightswatch(char **cmd, char num){			// executes nightwatch command after error handling
		int sec = 2;
		if(num < 2){
			fprintf(stderr, "nightswatch: too less arguments\n");
			return 1;
		}
		if(cmd[1][0] == '-' && (strlen(cmd[1]) == 1 || cmd[1][1] != 'n')){
			fprintf(stderr, "nightswatch: invalid option -- %s\n", cmd[1]);
			return 1;
		}
		else if(cmd[1][0] == '-'){
			if(strlen(cmd[1]) == 2 && num == 2){
				fprintf(stderr, "nightswatch: option requires an argument -- %s\n", cmd[1]);
				return 1;
			}

			int start_cmd = 0; sec = -1;
			if(strlen(cmd[1]) > 2){
				int n = strlen(cmd[1]); sec = 0;
				for(int i=2; i<n; i++){
					if(cmd[1][i] < '0' || cmd[1][i] > '9'){
						sec = -1; break;
					}
					sec = sec*10 + (cmd[1][i] - '0');
				}
				start_cmd = 3;
			}
			else if(num > 2){
				int n = strlen(cmd[2]); sec = 0;
				for(int i=0; i<n; i++){
					if(cmd[2][i] < '0' || cmd[2][i] > '9'){
						sec = -1; break;
					}
					sec = sec*10 + (cmd[2][i] - '0');
				}
				start_cmd = 4;
			}
			if(sec == -1){
				fprintf(stderr, "nightswatch: failed to parse argument\n");
				return 1;
			}
			if( start_cmd > num){
				fprintf(stderr, "nightswatch: requires a <command>\n");
				return 1;
			}
			if(start_cmd != num || (strcmp(cmd[start_cmd-1], "interrupt") !=0 && strcmp(cmd[start_cmd-1], "dirty") !=0)){
				for(int i=0; i<num; i++){
					fprintf(stderr, "%s ", cmd[i]);
				}
				fprintf(stderr, ": could not found\n");
				return 1;
			}
		}
		else if((strcmp(cmd[1], "interrupt") != 0 && strcmp(cmd[1], "dirty") != 0) || num > 2){
			for(int i=0; i<num; i++){
				fprintf(stderr, "%s ", cmd[i]);
			}
			fprintf(stderr, ": could not found\n");
			return 1;
		}

		clear();
		if(strcmp(cmd[num-1], "interrupt") == 0){
			return watch_interrupt(sec);
		}
		else
			return watch_dirty(sec);
		return 1;
	}