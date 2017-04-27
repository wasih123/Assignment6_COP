
CC = gcc
CFLAGS = -g

all:  sim

sim:  main_part1.o cache_part1.o
	$(CC) -o sim main_part1.o cache_part1.o -lm

main.o:  main_part1.c cache_part1.h
	$(CC) $(CFLAGS) -c main_part1.c

cache.o:  cache_part1.c cache_part1.h
	$(CC) $(CFLAGS) -c cache_part1.c
