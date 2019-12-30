#ifndef SHELL_H

	#define SHELL_H
	
	struct Proc{
		char *pname;
		int pid;
	};
	extern char *shell_path;
	extern int running_proc_size, max_proc, child_pid;
	extern struct Proc running[1024];
	int execute_cmd(char *, char*);

	char* get_shell_path(char*);
	char* cmd_without_tilde(char*);
	void display(char*);
	int quit();

	void signal_handler(int);

	char** split_cmd(char*, int*, char[]);
	void trim(char*, char*);

	int execute_pipe(char*);

	int execute_redirection(char*);

	int ex_echo(char **, int);

	int pwd();

	int cronjob(char **, int);

	int cd(char **, int, char*);

	int ls(char**, int, char*);

	int pinfo(char**, int, char*);

	int jobs(char**, int);

	int kjob(char**, int);
	int overkill();

	int ex_setenv(char**, int);
	int ex_unsetenv(char**, int);

	int fg(char**, int);

	int bg(char**, int);

	void insert_cmd(char*);
	void load_history();
	int recall_command(char**, int);
	void close_history();
	int history(char**, int);

	int watch_interrupt(int);
	int watch_dirty(int);
	int nightswatch(char**, char);

#endif