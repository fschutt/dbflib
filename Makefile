# C/C++ mixed object makefile
CXX=g++
CXXFLAGS=-s -fPIC -std=c++11 -O3
DEBUG=-g -Wall -Wextra
LDFLAGS=-lm -lstdc++

SRC=src
INC=include
OBJ=obj
EXT=c cc cxx cpp c++ C
INPUT=$(wildcard $(patsubst %,$(SRC)/*.%, $(EXT)))
OBJECT=$(patsubst %,$(OBJ)/%.o, $(notdir $(basename $(INPUT))))
OUTPUT=libdbf.so

.PHONY: all clean fclean re

all: $(OUTPUT)

$(OUTPUT): $(OBJECT)
	$(LINK.cc) -shared $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.* $(wildcard $(INC)/*.h)
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) $(DEBUG) -I$(INC) -o $@ -c $<
clean:
	rm -rf $(OUTPUT) $(OBJ)/