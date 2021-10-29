setup:
	gcc -std=gnu99 -g -Wall -o smallsh smallsh.c parser.c input.c launch.c status.c

clean:
	rm smallsh