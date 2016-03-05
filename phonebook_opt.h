#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define MAX_MEMORY_POOL_NODES 500000

typedef struct __DETAIL_ENTRY {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} detailEntry;

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    detailEntry *detail;
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct __MEMORY_POOL {
    entry *array[MAX_MEMORY_POOL_NODES];
    int nodeCounter;
} memPool;


entry *findName(char lastname[], entry *pHead);
entry *append(char lastName[], entry *e);
void initMemPool(void);
entry *mempoolalloc(void);
void addTheNodeToMemoryPool(unsigned int uIndex);
void mempoolfree(void);


#endif
