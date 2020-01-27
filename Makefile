CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Werror -pedantic
RM=rm
LDLIBS =-lm -lrt
all: clean main
main: main.c
	${CC} -o main ${CFLAGS} main.c ${LDLIBS}
.PHONY: clean all
clean:
	-${RM} main
debug: clean
	${CC} -o main ${CFLAGS} -g main.c ${LDLIBS}