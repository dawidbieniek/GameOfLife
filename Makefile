main:
	gcc main.c rules.c board.c simulation.c gameThread.c -lncurses -lpthread -DWIDE_MODE
