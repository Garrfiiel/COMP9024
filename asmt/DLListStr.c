/*
 Implementation of doubly-linked list ADT for string values.
 Written by Ashesh Mahidadia Jan 2020, based on code writted by John Shepherd 2015.
 You may need to modify the following implementation and test it properly before using
 in your program.
*/
// Created by Ziyi Wang on 2020/1/23.


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "DLListStr.h"

// data structures representing DLListStr

typedef struct DLListNode {
    char   *value;  // value of this list item (string)
    struct DLListNode *next;  // pointer to next node in list
    int outDegree;
    int count;
    double PRValue;
    DLListStr l;
} DLListNode;

typedef struct DLListRep {
    int  nitems;      // count of items in list
    DLListNode *first; // first node in list
    DLListNode *last;  // last node in list
} DLListRep;

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *val, int outDegree, double PRValue)
{
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);
    new->value = malloc(sizeof(strlen(val) + 1));
    assert(new->value != NULL);

    strcpy(new->value, val);  // for int, new->value = it;

    new->value[strlen(val)] = '\0';
    new->next = NULL;
    new->outDegree = outDegree;
    new->PRValue = PRValue;
    new->l = NULL;
    return new;
}

// create a new empty DLListStr
DLListStr newDLListStr()
{
    struct DLListRep *L;

    L = malloc(sizeof (struct DLListRep));
    assert (L != NULL);
    L->nitems = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// Reference https://blog.csdn.net/xufan0001/article/details/70225039
DLListNode *insertSort(DLListNode *head) {
    DLListNode *first, *t, *p, *q;
    first = head->next;
    head->next = NULL;
    while (first != NULL) {
        for (t = first, q = head; ((q != NULL) && (strcmp(q->value, t->value) < 0)); p = q, q = q->next);
        first = first->next;
        if (q == head) {
            head = t;
        } else {
            p->next = t;
        }
        t->next = q;
    }
    return head;
}

int compare(DLListNode *q, DLListNode *t) {
    if (q->count == t->count) {
        return q->PRValue > t->PRValue;
    } else {
        return q->count > t->count;
    }
}

DLListNode *insertSortPageRank(DLListNode *head) {
    assert(head != NULL);
    DLListNode *first, *t, *p, *q;
    first = head->next;
    head->next = NULL;
    while (first != NULL) {
        for (t = first, q = head; ((q != NULL) && (compare(q, t) > 0)); p = q, q = q->next);
        first = first->next;
        if (q == head) {
            head = t;
        } else {
            p->next = t;
        }
        t->next = q;
    }
    return head;
}

//  Insert sets in default order.
void insertSet(DLListStr L, char *val){
    assert(L != NULL);
    DLListNode *curr = L->first;
    while (curr != NULL) {
        if (strcmp(curr->value, val) == 0) {
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL) {

        DLListNode *newNode = newDLListNode(val, 0, 0.0);

        if (L->last == NULL) {
            L->first = L->last = newNode;
            L->nitems++;
        } else {
            L->last->next = newNode;
            L->last = L->last->next;
            L->nitems++;
        }
    }
}

// Insert sets with urls in default order.
void insertSetUrls(DLListStr L, char *val, char *url) {
    assert(L != NULL);
    DLListNode *curr = L->first;
    while (curr != NULL) {
        if (strcmp(curr->value, val) == 0) {
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL) {
        insertSet(L, val);
        curr = L->last;
    }

    if (curr->l == NULL) {
        curr->l = newDLListStr();
    }
    insertSet(curr->l, url);
}

void insertSetPageRank(DLListStr L,char *val, int outDegree, double PRValue) {
    assert(L != NULL);
    DLListNode *curr = L->first;
    while (curr != NULL) {
        if (strcmp(curr->value, val) == 0) {
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL) {
        DLListNode *newNode = newDLListNode(val, outDegree, PRValue);
        if (L->last == NULL) {
            L->first = L->last = newNode;
            L->nitems++;
        } else {
            L->last->next = newNode;
            L->last = L->last->next;
            L->nitems++;
        }
    }
}

void sortDLListStr(DLListStr L) {
    L->first = insertSort(L->first);
}

void sortDLListStrPageRank(DLListStr L) {
    L->first = insertSortPageRank(L->first);
}

// display items from a DLListStr, space separated
void showDLListStr(DLListStr L)
{
    assert(L != NULL);
    DLListNode *curr, *url;
    for (curr = L->first; curr != NULL; curr = curr->next) {
        fprintf(stdout, "%s", curr->value);
        if (curr->l != NULL) {
            for (url = curr->l->first; url != NULL; url = url->next) {
                fprintf(stdout, " %s", url->value);
            }
        }
        fprintf(stdout, "\n");
    }
}

// copy inverted list to the txt file
void copyDLListStr(DLListStr L, FILE *fp) {
    assert(L != NULL);
    assert(fp != NULL);
    DLListNode *curr, *url;
    for (curr = L->first; curr != NULL; curr = curr->next) {
        fprintf(fp, "%s", curr->value);
        if (curr->l != NULL) {
            for (url = curr->l->first; url != NULL; url = url->next) {
                fprintf(fp, " %s", url->value);
            }
        }
        fprintf(fp, "\n");
    }
}

// free up all space associated with list
void freeDLListStr(DLListStr L)
{
    assert(L != NULL);
    DLListNode *curr, *prev;
    curr = L->first;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        if (prev->l != NULL) {
            freeDLListStr(prev->l);
        }
        free((prev->value));
        free(prev);
    }
    free(L);
}

void searchDLListStr(int i, char *argv[], DLListStr pageRank, DLListStr invertedIndex, DLListStr finalRank) {

    assert(pageRank != NULL);
    assert(invertedIndex != NULL);
    assert(finalRank != NULL);

    DLListNode *invertedIndexHead;

   invertedIndexHead = invertedIndex->first;
   while (invertedIndexHead != NULL) {
       if (strcmp(argv[i], invertedIndexHead->value) == 0) {
           if (invertedIndexHead->l != NULL) {
               DLListNode *urlNode = invertedIndexHead->l->first;
               DLListNode *rankNode;
               while (urlNode != NULL) {
                   rankNode = pageRank->first;
                   while (rankNode != NULL) {
                       // count
                       if (strcmp(urlNode->value, rankNode->value) == 0) {
                           DLListNode *curr = finalRank->first;
                           while (curr != NULL) {
                               if (strcmp(curr->value, urlNode->value) == 0) {
                                   break;
                               }
                               curr = curr->next;
                           }
                           if (curr == NULL) {
                               insertSet(finalRank, urlNode->value);
                               curr = finalRank->first;
                               while (curr != NULL) {
                                   if (strcmp(curr->value, urlNode->value) == 0) {
                                       break;
                                   }
                                   curr = curr->next;
                               }
                           }
                           curr->count++;
                           curr->PRValue = rankNode->PRValue;
                           break;
                       }
                       rankNode = rankNode->next;
                   }
                   urlNode = urlNode->next;
               }
           }
       }
       invertedIndexHead = invertedIndexHead->next;
   }
}

void showFinalRank(DLListStr L) {
    DLListNode *node = L->first;
    int count = 0;
    while ((node != NULL) && (count < 30)) {
        printf("%s\n", node->value);
        node = node->next;
        count++;
    }
}

