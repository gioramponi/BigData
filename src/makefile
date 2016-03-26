CC = gcc
FLAGS = -O3
DIR = ../bin/

all: make-rand-file file-scan scan-sort

make-rand-file: multiwayMergesort common.h make-rand-file.c
	$(CC) $(FLAGS) make-rand-file.c -I$(DIR) $(DIR)multiwayMergesort.o -o $(DIR)make-rand-file

file-scan: file-scan.c
	$(CC) $(FLAGS) file-scan.c -o $(DIR)file-scan

multiwayMergesort: multiwayMergesort.h multiwayMergesort.c
	$(CC) $(FLAGS) multiwayMergesort.c -c -o $(DIR)multiwayMergesort.o

scan-sort: common.h multiwayMergesort scan-sort.c
	$(CC) $(FLAGS) scan-sort.c -I$(DIR) $(DIR)multiwayMergesort.o -o $(DIR)scan-sort