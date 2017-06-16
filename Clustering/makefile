CC = g++
SOURCES = main.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -std=c++17

all: main

main: main.cpp
	$(CC) $(CFLAGS) $(SOURCES) -o clusterSingle.o
test:
	$(CC) $(CFLAGS) test.cpp -o test.o


clean: 
	rm -rf *o *out
	clear