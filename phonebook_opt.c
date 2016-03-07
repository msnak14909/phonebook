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
int slotIndx = 0;
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

int hash(char *str)
{
    unsigned long hash = 5381;
    while (*str++)
        hash = ((hash << 5) + hash) + *str; /* hash * 33 + c */
    return (int)(hash % HASH_TABLE_SIZE);
}
entry *append(char lastName[], entry **e)
{
    if(!lastName) return NULL;
    int key = hash(lastName);
    entry *cur;
    if(!(cur = mempoolalloc())) return NULL;

    memcpy(cur->lastName, lastName, sizeof(cur->lastName));

    if(e[key]) {
        cur->pNext = e[key]->pNext;
        e[key]->pNext = cur;
    } else {
        cur->pNext = NULL;
        e[key]=cur;
    }
    return cur;
}
entry *findName(char lastName[], entry **e)
{
    if(!lastName) return NULL;
    unsigned long key = hash(lastName);
    entry *t = e[key];
    while(t && strcasecmp(lastName, t->lastName)) {
        t = t->pNext;
    };
    //if(t) printf("founded: %s\n", t->lastName);
    return t ? t : NULL;
}


