// DLListStr.h - Interface to doubly-linked list ADT
// Written by John Shepherd, March 2013
// Last modified, Ashesh Mahidadia, Jan 2020
// Created by Ziyi Wang on 2020/1/23.


#ifndef ASS_DLLISTSTR_H
#define ASS_DLLISTSTR_H

#include <stdio.h>
//#include "DLListStr.h"

// External view of DLListStr
// Implementation given in DLListStr.c
// Implements a DLListStr of string

typedef struct DLListRep *DLListStr;



// create a new empty DLListStr
DLListStr newDLListStr();

void insertSet(DLListStr, char *);

// insert another list
void insertSetUrls(DLListStr, char *, char *);

void insertSetPageRank(DLListStr, char *, int, double);

void searchDLListStr(int, char **, DLListStr, DLListStr, DLListStr);

void sortDLListStr(DLListStr);

void sortDLListStrPageRank(DLListStr);

// free up all space associated with list
void freeDLListStr(DLListStr);

// display items from a DLListStr, comma separated
void showDLListStr(DLListStr);

void showFinalRank(DLListStr);

void copyDLListStr(DLListStr, FILE *);

#endif //ASS_DLLISTSTR_H
