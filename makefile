CC = gcc
FLAGS = -O3
DIR = bin/
SRC = src/

all: pre

pre:
	if [ ! -d "$(DIR)" ]; then mkdir $(DIR); fi
	cd $(SRC); make

4gb.dat:
	./$(DIR)make-rand-file $(DIR)4gb.dat 1000000000

8gb.dat:
	./$(DIR)make-rand-file $(DIR)8gb.dat 2000000000