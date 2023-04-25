#DEFINES=-DWIDE_MODE
WARNINGS=-ansi -pedantic
OBJECTS=rules.o board.o simulation.o gameThread.o gameMenu.o inputWindow.o
LIBS=-lncurses -lmenu -lpthread
NAME=gol

CC=gcc
CFLAGS=${WARNINGS}
CPPFLAGS=${DEFINES}

ifeq (${WIDE}, 1)
	CPPFLAGS+= -DWIDE_MODE
endif

ifeq (${REDRAW}, 1)
	CPPFLAGS+= -DFULL_REDRAW
endif

# Main program
main: ${OBJECTS}
	${CC} ${CFLAGS} ${CPPFLAGS} -o ${NAME} main.c ${OBJECTS} ${LIBS}

inputWindow.o: inputWindow.c

gameMenu.o: gameMenu.c

gameThread.o: gameThread.c board.o rules.o

simulation.o: simulation.c board.o rules.o

board.o: board.c

rules.o: rules.c

clean:
	rm *.o
