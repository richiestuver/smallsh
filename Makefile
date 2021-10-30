setup:
	gcc -std=gnu99 -g -Wall -o smallsh smallsh.c parser.c input.c launch.c status.c builtins.c command.c sighandlers.c exec_env.c

clean:
	rm smallsh