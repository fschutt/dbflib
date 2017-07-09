CC = g++
CFLAGS = -O3 -Wall -Werror 
INC=-I ./include/ 
CXXFLAGS = $(CFLAGS) $(INC) -o main
SRCS = $(wildcard src/*.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))

all: $(PROGS)
	$(CC) $(CFLAGS)

clean: 
	rm -f $(PROGS)