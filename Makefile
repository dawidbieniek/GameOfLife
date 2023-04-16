main:
	gcc main.c rules.c board.c simulation.c gameThread.c gameMenu.c inputWindow.c -lncurses -lpthread -lmenu -DWIDE_MODE
