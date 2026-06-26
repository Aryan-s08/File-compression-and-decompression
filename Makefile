CC = g++
CFLAGS = -Wall

TARGETS = main compress decompress

all: $(TARGETS)

main: main.c
	gcc $(CFLAGS) main.c -o main

compress: compress.cpp
	$(CC) $(CFLAGS) compress.cpp -o compress

decompress: decompress.cpp
	$(CC) $(CFLAGS) decompress.cpp -o decompress

clean:
	rm -f $(TARGETS)