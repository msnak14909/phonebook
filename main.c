#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL
#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* setting mempool and build the entry*/
#if defined(OPT)
    initMemPool();
    printf("size of entry : %lu bytes\n", sizeof(entry));
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(OPT)
#if defined(__GNUC__)
    __builtin___clear_cache((char *) HashTable, (char *) HashTable + sizeof(entry));
#endif
#else
#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif

#if defined(OPT)
#if defined(SLOT)
    int hash_slot[HASH_TABLE_SIZE] = {0};
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#if defined(SLOT)
        hash_slot[hash(line)]++;
#endif
        if(append(line, HashTable)==NULL)
            puts("apped failed, the lastName is not valid.");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);


    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    assert(findName(input, HashTable) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, HashTable)->lastName, "zyxel"));
#else

    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        if((e = append(line, e))==NULL)
            puts("apped failed, the lastName is not valid.");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    e = pHead;

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = pHead;

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(OPT)
#if defined(__GNUC__)
    __builtin___clear_cache((char *) HashTable, (char *) HashTable + sizeof(entry));
#endif
#else
#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif
    /* compute the execution time */
#if defined(OPT)
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, HashTable);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);
#else
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);
#endif
    FILE *output;
#if defined(OPT)
    output = fopen("opt.txt", "a");
#if defined(SLOT)
    FILE *output2;
    output2 = fopen("slot.txt", "a");
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        fprintf(output2, "%d %d\n", i, hash_slot[i]);
    }
    fclose(output2);
#endif

#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %.9lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %.9lf sec\n", cpu_time2);

#if defined(OPT)
    mempoolfree();
#else
    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#endif
    return 0;
}
