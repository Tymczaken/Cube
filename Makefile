CC=gcc
LDFLAGS= -lm -LGL -lglut -lGL

main:
	${CC} main.c ${LDFLAGS} -o cube -g
