# Shell in C

This is a linux shell written in C with basic functionalities like inbuilt commands (eg. `ls` and `cd`) along with user-defined functions (eg. `pinfo`) and normal terminal commands that are executed in it. It has the capability to run both background and foreground processes. It can also handle redirection and piping. Also takes in semicolon separated commands. Appropriate signal handling has also been added.

## RUNNING

- `cd` into directory.

- sudo apt-get install libncurses5-dev libncursesw5-dev;sudo apt install ncurses-doc 

- run `make`

- to delete '.o' files, run `make clean`

- type `exit` or `quit` to exit the shell

## Working Of Shell:

1.  Displays `username@systemname:current_directory`. Home Directory of shell is indicated by `~`
2.  `echo`, `cd`, `pwd`, `ls`, `pinfo`, `history`, `nightswatch`, `jobs`, `kjobs`, `overkill`, `quit`, `fg`, `bg`, `cronjob`, `setenv`, `unsetenv` is build manually.
3.  `pwd`. Prints present working directory.
4.  `cd <directoryPath>(optional)`. Changes the directory to `<directoryPath>`. For relative path only ../ or ./ or ~/ is allowed. Default directoryPath is `pwd`.
5.  `ls <flags>(optional) <directoryPath>(optional)`. Lists all the files and folders in the directory. Flags can be '-l' (to show long listing format) or '-a' (to show hidden files also). For relative path only ../ or ./ or ~/ is allowed. Default directoryPath is `pwd`.
6.  `pinfo <pid>` (optional). Outputs:- pid, Process Status, memory, Executable Path
7.  `history <num>` (optional). Prints last max(num, 10) commands where num <= 20.
8.  `nightswatch <options> <command>`. Modified version of watch command. Ex: 'nightswatch -n 2 `<command>`'. Command can be:- interrupt, dirty. Press q for exit.
9.  `setenv <var> [value](optional)`. Sets environment variable with value. Default value is "". Ex: setenv x [2].
10. `unsetenv <var>`. Unsets environment variable.
11. `jobs`. Prints all currently existing jobs with their job_id and pid and job_name and its status.
12. `kjobs <job_id> <sigalNumber>`. Kills job with given signal.
13. `overkill`. Kills all background process with SIGINT.
14. `fg <job_id>`. Brings the background process in the foreground.
15. `bg <job_id>`. Runs the background process.
16. `quit`. Quits the shell.
17. `cronjob -t <num> -c [command] -p <num>`. Executes a particular command in fixed time interval for a certain period. Ex: 'cronjob -c [ls] -t 3 -p 6'. This command executes ‘ls’ command after every 3 seconds until 6 seconds are elapsed.
18. Handles redirection and piping with proper Signal Handling.

## FILES
- main.c : main file which contains shell loop.
- execute_piping.c : handles piping and calls execute_redirection().
- execute_redirection.c : handles redirection and calls execute_cmd().
- execute_cmd.c : contains execution loop, calls every function based on user command.
- parse.c : contaings two functions 'split_cmd' and 'trim'.
- cd.c : changes directory.
- echo.c : echoes whatever the user typed.
- ls.c : displays list of all files and directories.
- pwd.c : displays present working directory.
- pinfo.c : prints the process related info.
- history.c : prints the previous commands given.
- setenv.c : sets or unsets environment variable.
- jobs.c : prints all the current jobs.
- kjob.c : contains 2 functions, 'kjob' to kill the job and 'overkill' to kill all background jobs.
- fg.c : brings the process to foreground.
- bg.c: changes status of background process to running. 
- nightswatch.c : nightswatch executes the command after every n seconds until q is pressed.
- cronjob.c : This command executes ‘command’ after every t seconds until p seconds are elapsed.
- util.c : contains 'quit', 'display', 'signal_handler', 'get_shell_path', 'cmd_without_tilde' functions. 
- shell.h : header file, contains definitions of the functions and structs and global variables used in above files.
