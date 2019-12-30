CC=gcc
CFLAGS=-I ./ -lncurses
DEPS=shell.h
OBJ= main.o cd.o echo.o cronjob.o setenv.o fg.o bg.o kjob.o jobs.o execute_cmd.o execute_piping.o execute_redirection.o history.o ls.o nightswatch.o parse.o pinfo.o pwd.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
# 	here -o $@ denotes left side of (:) and $< is the first item on the right side of (:)

# all: clean
shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
# 	here $^ denotes all the items which are on the right side of (:)

.PHONY: clean

clean:
	rm -f *.o 