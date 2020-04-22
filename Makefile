ifdef DEBUG
	CFLAG = -DDEBUG -WALL -std=c99
endif
CFLAG ?= -WALL -std=c99

main: main.o scheduler.o process.o
	gcc $(CFLAG) main.o scheduler.o process.o -o main

main.o: main.c
	gcc $(CFLAG) main.c -c

scheduler.o: scheduler.c
	gcc $(CFLAG) scheduler.c -c

process.o: process.c
	gcc $(CFLAG) process.c -c

.PHONY = clean

clean:
	rm -rf *.o main

run:
	sudo ./main