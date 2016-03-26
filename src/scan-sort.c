#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "multiwayMergesort.h"
#include "common.h"

void scan(const char* filename, size_t size, unsigned B, void (*taks)(const void*, int));

/* type of file items */
typedef unsigned int ItemType;

// comparator for ItemType integers
int compar(const void* a, const void* b){
    return *(ItemType*)a-*(ItemType*)b;
}

int pair_compare_first(const void* a, const void*b)
{
    return (*(pair*)a).a-(*(pair*)b).a;
}

int pair_compare_second(const void* a, const void*b)
{
    return (*(pair*)a).b-(*(pair*)b).b;
}

FILE *f, *f1;

const unsigned B = 1048576;
const int M = 32;

void produce_pair(const void *a, int c)
{
    pair_typetype  current = *((pair_typetype*)a);
    pair p;
    p.a = current;
    p.b  =c;
    fwrite(&p, sizeof(pair), 1, f);
}

void produce_output(const void *a, int c)
{
    pair current = *((pair*)a);
    ItemType val = (ItemType)current.a; //safe here
    fwrite(&val, sizeof(ItemType), 1, f);
}

void* buf;
int toread = 0; //se sforo questo sto uscendo da buf

void replace_pair(const void *a, int c)
{
    if((c%B) == 0){
        int red;
        //printf("%d %d\n", B*sizeof(ItemType), B);
        if ((red = fread(buf, 1, sizeof(ItemType)*B, f1)) < 0)exit((printf("file read error: %d\n", red), 1));
        toread = red / sizeof(ItemType);
        //DEBUG
        //printf("READ! %d\n", red);
    }
    if(toread-- <= 0)
        exit((printf("buf read outofrange: %d\ncurrent: %d\n", toread, c), 1));
    pair p = *((pair*)a);
    ItemType pp = ((ItemType*)buf)[c%B];
    p.a = pp;
    fwrite(&p, sizeof(pair), 1, f);
}




// main function
int main(int argc, char** argv){

    int theErrCode;

    /* check command line parameters */
    if (argc < 2)
        exit((fprintf(stderr,
              "Usage: %s fileName \n", argv[0])));
    
    char name[255];
    bzero(name, 255);
    strcpy(name, argv[1]);
    strcat(name, "_perm");
    
    f= fopen("C_PAIR", "w+");
    if (f == NULL) exit(printf("can't open file\n"));
    //PHASE1
    printf("PHASE1");
    scan(name, sizeof(pair_typetype), B, &produce_pair);
    fclose(f);
    printf("...[OK]\n");
    
    //PHASE2
    printf("PHASE2");
    theErrCode = sort("C_PAIR", sizeof(pair),
         &pair_compare_first, B, M, 1, 0);
    // check if the call failed
    if (theErrCode != 0)
        fprintf(stderr, "%s\n", GetErrorString(theErrCode));
    printf("...[OK]\n");
    
    //PHASE3
    printf("PHASE3");
    
    //printf("\n%s\n", argv[1]);
    
    buf = malloc(B*sizeof(ItemType));
    if (buf == NULL) exit((printf("out of memory\n"), 1));
    
    f1= fopen(argv[1], "r");
    if (f1 == NULL) exit(printf("can't open file\n"));
    
    f= fopen("REPLACED_PAIR", "w+");
    if (f == NULL) exit(printf("can't open file\n"));
    
    scan("C_PAIR", sizeof(pair), B, &replace_pair);
    
    fclose(f1);
    fclose(f);
    free(buf);
    printf("...[OK]\n");
    //PHASE4
    printf("PHASE4");
    theErrCode = sort("REPLACED_PAIR", sizeof(pair),
                      &pair_compare_second, B, M, 1, 0);
    /* check if the call failed */
    if (theErrCode != 0)
        fprintf(stderr, "%s\n", GetErrorString(theErrCode));
    printf("...[OK]\n");
    //PHASE5
    char name_out[255];
    bzero(name_out, 255);
    strcpy(name_out, argv[1]);
    strcat(name_out, "_out");
    printf("PHASE5");
    f= fopen(name_out, "w+");
    if (f == NULL) exit(printf("can't open file\n"));
    
    scan("REPLACED_PAIR", sizeof(pair), B, &produce_output);

    fclose(f);
    printf("...[OK]\n");
    printf("CLEANING");
    unlink("REPLACED_PAIR");
    unlink("C_PAIR");
    printf("...[OK]\n");
    return theErrCode;
}

void scan(const char* filename, size_t size, unsigned B, void (*task)(const void*, int))
{
    void *buf = malloc(B*size);
    if (buf == NULL) exit((printf("out of memory\n"), 1));
    
    FILE *f1 = fopen(filename, "r");
    if (f1 == NULL) exit(printf("can't open file\n"));
    int tot = 0;
    //size = sizeof(pair)
    for (;;) {
        pair val;
        int red;
        if ((red = fread(buf, 1, B*size, f1)) < 0)
            exit((printf("file read error: %d\n", red), 1));
        if(red == 0)
            break;
        unsigned pred = 0;
        int j = 0;
        for(j = 0; j < red/size; j++)
        {
            void *current = buf+(j*size);
            (*task)(current, tot);
            tot++;
        }
        printf(".");
    }
    fclose(f1);
}
