CC = gcc

systemStats: main.o cla_functions.o stats_functions.o
	$(CC) -o systemStats main.o cla_functions.o stats_functions.o

main.o: main.c cla_functions.o stats_functions.o main.h
	$(CC) -c main.c

cla_functions.o: cla_functions.c cla_functions.h
	$(CC) -c cla_functions.c

stats_functions.o: stats_functions.c stats_functions.h
	$(CC) -c -w stats_functions.c

clean:
	rm -f systemStats
	rm -f main.o
	rm -f cla_functions.o
	rm -f stats_functions.o
