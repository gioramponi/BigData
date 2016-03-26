#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "multiwayMergesort.h"

/* type of file items */
typedef unsigned item_type;
typedef pair pair_type;

int pair_compare(const void* a, const void*b)
{
    return (*(pair_type*)a).a-(*(pair_type*)b).a;
}

int main(int argc, char** argv){

	FILE* f,*f1;
	long long N, i;
	
	/* check command line parameters */
	if (argc < 3) exit (printf("Usage: %s file-name num-items\n", argv[0]));
	
	/* convert number of items from string to integer format */
	N = atoll(argv[2]);
	
	printf("file offset model: %lu bit\n", sizeof(off_t)*8);
	printf("creating random file of %lld %lu bit integers...\n", N, sizeof(item_type)*8);
	
	/* open file for writing */
	f = fopen(argv[1], "w+");
	if (f == NULL) exit(printf("can't open file\n"));
    
    char perm[255];
    bzero(perm, 255*sizeof(char));
    strcpy(perm, argv[1]);
    strcat(perm, "_pairtmp");
    
    f1 = fopen(perm, "w+");
    if (f1 == NULL) exit(printf("can't open file\n"));

	/* make N sequential file accesses */
	for (i=0; i<N; ++i) {
	    item_type val = rand();
        pair pairval;
        pairval.a = rand();
        pairval.b = i;
	    fwrite(&val, sizeof(item_type), 1, f);
        fwrite(&pairval, sizeof(pair), 1, f1);
	}
    
    fclose(f1);
	fclose(f);
    
    printf("now creating permutation...\n");
    
    const int B = 1048576;
    const int M = 32;
    
    int theErrCode = sort(	perm, sizeof(pair_type),
                      pair_compare, B, M, 1, 0);
    
    /* check if the call failed */
    if (theErrCode != 0)
        fprintf(stderr, "%s\n", GetErrorString(theErrCode));

    
    f1 = fopen(perm, "r");
    if (f1 == NULL) exit(printf("can't open file\n"));
    
    printf("sort completed\n");
    
    char perm1[255];
    bzero(perm1, 255*sizeof(char));
    strcpy(perm1, argv[1]);
    strcat(perm1, "_perm");
    
    f = fopen(perm1, "w+");
    if (f == NULL) exit(printf("can't open file\n"));
    
    void *buf = malloc(B*sizeof(pair_type));
    if (buf == NULL) exit((printf("out of memory\n"), 1));
    
    for (;;) {
        pair_type val;
        int red;
        if ((red = fread(buf, 1, B*sizeof(pair_type), f1)) < 0)
            exit((printf("file read error: %d\n", red), 1));
            if(red == 0)
            	break;
        unsigned pred = 0;
        int j = 0;
        for(j = 0; j < red/sizeof(pair_type); j++)
        {
            pair_type current = ((pair_type*)(buf))[j];
            fwrite(&(current.b), sizeof(pair_typetype), 1, f);
        }
        printf(".");
    }
    printf("\n");
    free(buf);
    fclose(f1);
    unlink(perm);
    fclose(f);

}


