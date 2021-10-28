setup:
	gcc -std=gnu99 -g -Wall -o smallsh smallsh.c parser.c input.c launch.c

clean:
	rm smallsh