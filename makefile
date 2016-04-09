CC = gcc
FLAGS = -O3
DIR = bin/
SRC = src/

all: pre

pre:
	if [ ! -d "$(DIR)" ]; then mkdir $(DIR); fi
	cd $(SRC); make

1gb.dat:
	./$(DIR)make-rand-file $(DIR)1gb.dat 250000000

2gb.dat:
	./$(DIR)make-rand-file $(DIR)2gb.dat 500000000

4gb.dat:
	./$(DIR)make-rand-file $(DIR)4gb.dat 1000000000

8gb.dat:
	./$(DIR)make-rand-file $(DIR)8gb.dat 2000000000
16gb.dat:
	./$(DIR)make-rand-file $(DIR)16gb.dat 4000000000
32gb.dat:
	./$(DIR)make-rand-file $(DIR)32gb.dat 8000000000
