
CC = gcc
CFLAGS = -g

all:  sim

sim:  main_part2.o cache_part2.o
	$(CC) -o sim main_part2.o cache_part2.o -lm

main.o:  main_part2.c cache_part2.h
	$(CC) $(CFLAGS) -c main_part2.c

cache.o:  cache_part2.c cache_part2.h
	$(CC) $(CFLAGS) -c cache_part2.c
