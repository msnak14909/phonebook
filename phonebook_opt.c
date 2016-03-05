#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */

/* original version */
entry *findName_orig(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append_orig(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    return e;
}

/* opt 1: change the struct size */
entry *findName_opt1(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append_opt1(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    memcpy(e->lastName, lastName, sizeof(e->lastName));
    e->pNext = NULL;
    return e;

}

/* opt 2: change the malloc to memory pool*/
memPool allocNode;
unsigned int slotIndx = 0;
void initMemPool(void)
{
    unsigned int uIndex;
    for(uIndex = 0; uIndex < MAX_MEMORY_POOL_NODES; uIndex++) {
        allocNode.array[uIndex] = (entry*)malloc(sizeof(entry));
        allocNode.nodeCounter++;
    }
}

entry *mempoolalloc(void)
{
    if(slotIndx == MAX_MEMORY_POOL_NODES) {
        printf("No free Nodes are available\n");
        return NULL;
    } else {
        slotIndx++;
        allocNode.nodeCounter--;
    }
    return allocNode.array[slotIndx];
}

void mempoolfree(void)
{
    unsigned int uIndx;
    for(uIndx = 0; uIndx < MAX_MEMORY_POOL_NODES; uIndx++) {
        free(allocNode.array[uIndx]);
    }
}

entry *findName_opt2(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append_opt2(char lastName[], entry *e)
{
    if((e->pNext = mempoolalloc())==NULL)
        return NULL;
    e = e->pNext;
    memcpy(e->lastName, lastName, sizeof(e->lastName));
    e->pNext = NULL;
    return e;
}
/* opt3 hasing function */

unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)!=0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
entry *append(char lastName[], entry **e)
{
    unsigned long key = hash(lastName) % HASH_TABLE_SIZE;
    entry *cur, *t;
    if(!e[key]) {
        if(!(e[key] = mempoolalloc()))
            return NULL;
        memcpy(e[key]->lastName, lastName, sizeof(e[key]->lastName));
        e[key]->pNext = NULL;
        return e[key];
    }
    cur = mempoolalloc();
    t = e[key]->pNext;
    cur->pNext = t;
    memcpy(cur->lastName, lastName, sizeof(cur->lastName));
    e[key]->pNext = cur;
    return cur;
}
entry *findName(char lastName[], entry **e)
{
    unsigned long key = hash(lastName) % HASH_TABLE_SIZE;
    entry *t;
    t = e[key];
    while(t && strcasecmp(lastName, t->lastName)) {
        t = t->pNext;
    };
    return t ? t : NULL;
}
