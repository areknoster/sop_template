CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Werror -pedantic
RM=rm
LDLIBS =-lm -lrt
MAINFILE=main
all: clean ${MAINFILE}
${MAINFILE}: ${MAINFILE}.c
	${CC} -o ${MAINFILE} ${CFLAGS} ${MAINFILE}.c ${LDLIBS}
.PHONY: clean all
clean:
	-${RM} ${MAINFILE}
debug: clean
	${CC} -o ${MAINFILE} ${CFLAGS} -g ${MAINFILE}.c ${LDLIBS}
