setup:
	gcc -std=gnu99 -g -Wall -o smallsh smallsh.c parser.c input.c

clean:
	rm smallsh