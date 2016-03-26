#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* type of file items */
typedef unsigned item_type;

int main(int argc, char** argv){

	FILE* f, *f1, *f2;
	long long i, N, B;
	void* buf, *perm_buf, *out_buf;

	/* check command line parameters */
	if (argc < 2) exit(printf("Usage: %s file-name\n", argv[0]));

	/* convert block size from string to integer format */
    B = 4096;//atoll(argv[2]);

	/* open file for reading */
	f = fopen(argv[1], "r");
	if (f == NULL) exit((printf("can't open file %s\n", argv[1]), 1));
    
    char perm_name[255];
    memset(perm_name, 0, 255);
    strcpy(perm_name, argv[1]);
    strcat(perm_name, "_perm");
    
    f1 = fopen(perm_name, "r");
    if (f1 == NULL) exit((printf("can't open file %s\n", perm_name), 1));

    char out_name[255];
    memset(out_name, 0, 255);
    strcpy(out_name, argv[1]);
    strcat(out_name, "_out");
    
    f2 = fopen(out_name, "r");
    if (f2 == NULL) exit((printf("can't open file %s\n", out_name), 1));

	/* allocate block */
	buf = malloc(B*sizeof(item_type));
	if (buf == NULL) exit((printf("out of memory\n"), 1));
    
    perm_buf = malloc(B*sizeof(long long));
    if (perm_buf == NULL) exit((printf("out of memory\n"), 1));
    
    out_buf = malloc(B*sizeof(item_type));
    if (out_buf == NULL) exit((printf("out of memory\n"), 1));

	/* compute number N of elements in the file */
	fseeko(f, 0LL, SEEK_END);
	N = ftello(f)/sizeof(item_type);
    fseeko(f, 0LL, SEEK_SET);

	printf("file offset model: %lu bit\n", sizeof(off_t)*8);
	printf("block size: %llu items\n", B);
	printf("make block reads from file of %lld %lu bit integers...\n",
		N, sizeof(item_type)*8);

    int total = 0;
    
    printf("#\t\tIN\tPERM\tOUT\n\n");
    
	/* make R random block accesses */
	for (;;) {
        if(total == N)
            break;
	    item_type val;
        int red;
        if ((red = fread(buf, 1, B*sizeof(item_type), f)) == 0){
            printf("file read error: %d\n", red);
            break;}
        if ((fread(perm_buf, 1, B*sizeof(long long), f1)) == 0){
            printf("file read error: %d\n", red);
            break;}
        if ((fread(out_buf, 1, B*sizeof(item_type), f2)) == 0){
            printf("file read error: %d\n", red);
            break;}
        int j = 0;
        for(j = 0; j < red/sizeof(item_type); j++)
        {
            unsigned current = ((item_type*)(buf))[j];
            long long current_perm = ((long long*)(perm_buf))[j];
            unsigned current_out = ((item_type*)(out_buf))[j];
            printf("%d\t\t%u\t%lld\t%d\n", total, current, current_perm, current_out);
            total++;
        }
	}

	free(buf);
	fclose(f);
    fclose(f1);
    fclose(f2);
}
