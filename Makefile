
build: 2048

2048: 2048.c
        gcc -Wall 2048.c -o 2048 -lncurses


.PHONY:

clean:
        rm -f 2048

run:
