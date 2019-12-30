#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include "shell.h"


	int ls(char** cmd, int num, char* shell_path){					// lists all the files and directories
		char *path = (char*) malloc((PATH_MAX+4)*sizeof(char));
		int is_path = 0, is_flag_l = 0, is_flag_a = 0;

		for(int i=1; i<num; i++){
			if(cmd[i][0] == '-'){
				for(int j=1; j<strlen(cmd[i]); j++){
					if(cmd[i][j] == 'l') is_flag_l=1;
					if(cmd[i][j] == 'a') is_flag_a=1; 
				}
			}
			else{
				is_path = 1;
				strcpy(path, cmd[i]);
			}
		}
		if(!is_path){
			getcwd(path, PATH_MAX+4);
		}

		struct stat st;
		stat(path, &st);
		if(!S_ISDIR(st.st_mode)){
			fprintf(stderr, "Error: not a directory\n");
			free(path);
			return 1;
		}
 
		DIR *dir = opendir(path);							// reads directory
		if(dir == NULL){
			perror("Error in opendir");
			free(path);
			return 1;
		}
		struct dirent* file;
		while((file = readdir(dir))){
			if(!is_flag_a && file->d_name[0] == '.'){
				continue;
			}

			char *f_path = (char*) malloc((PATH_MAX+4)*sizeof(char));
			strcpy(f_path, path);
			if(path[strlen(path)-1] != '/') strcat(f_path,"/");
			strcat(f_path, file->d_name);

			if(stat(f_path, &st) == -1){
				perror("Error getting file info");
				free(path);free(f_path);
				return 1;
			}

			char *f_permissions = (char*) malloc(11*sizeof(char));
			if(S_ISREG(st.st_mode)) f_permissions[0] = '-';
			else if(S_ISDIR(st.st_mode)) f_permissions[0] = 'd';
			else if(S_ISCHR(st.st_mode)) f_permissions[0] = 'c';
			else if(S_ISBLK(st.st_mode)) f_permissions[0] = 'b';
			else if(S_ISFIFO(st.st_mode)) f_permissions[0] = 'p';
			else if(S_ISLNK(st.st_mode)) f_permissions[0] = 'l';
			else f_permissions[0] = 's';

			if(st.st_mode & S_IRUSR) f_permissions[1] = 'r';
			else f_permissions[1] = '-';
			if(st.st_mode & S_IWUSR) f_permissions[2] = 'w';
			else f_permissions[2] = '-';
			if(st.st_mode & S_IXUSR) f_permissions[3] = 'x';
			else f_permissions[3] = '-';

			if(st.st_mode & S_IRGRP) f_permissions[4] = 'r';
			else f_permissions[4] = '-';
			if(st.st_mode & S_IWGRP) f_permissions[5] = 'w';
			else f_permissions[5] = '-';
			if(st.st_mode & S_IXGRP) f_permissions[6] = 'x';
			else f_permissions[6] = '-';

			if(st.st_mode & S_IROTH) f_permissions[7] = 'r';
			else f_permissions[7] = '-';
			if(st.st_mode & S_IWOTH) f_permissions[8] = 'w';
			else f_permissions[8] = '-';
			if(st.st_mode & S_IXOTH) f_permissions[9] = 'x';
			else f_permissions[9] = '-';
			f_permissions[10] = '\0';

			char* f_mtime = (char*)malloc(sizeof(char)*100);
			strftime(f_mtime, 100, "%b %d %H:%M", localtime(&st.st_mtime));

			if (is_flag_l)
				printf("%s\t%d\t%s\t%s\t%d\t%s\t%s\n", f_permissions, (int)st.st_nlink, getpwuid(st.st_uid)->pw_name, getgrgid(st.st_gid)->gr_name, (int)st.st_size, f_mtime, file->d_name);
			else 
				printf("%s\n", file->d_name);
			free(f_permissions);
			free(f_path);
		}

		closedir(dir);
		free(path);
		return 1;
	}