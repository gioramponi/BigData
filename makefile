CC = gcc
FLAGS = -O3
DIR = bin/
SRC = src/

all: pre

pre:
	if [ ! -d "$(DIR)" ]; then mkdir $(DIR); fi
	cd $(SRC); make

4gb.dat:
	./$(BIN)make-rand 4gb.dat 1000000000

8gb.dat:
	./$(BIN)make-rand 8gb.dat 2000000000