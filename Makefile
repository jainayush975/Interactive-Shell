main: main.c mycommands.c
	gcc main.c mycommands.c -o main
clean:
	rm mycommands.o
