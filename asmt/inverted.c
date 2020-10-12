//
// Created by Ziyi Wang on 2020/1/28.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DLListStr.h"

#define MAXSTRING 1000

void scanS2(char **c, int vertices, DLListStr list) {
    char delim[6] = " .,;?";
    FILE *fp;
    char *token;
    char urlFile[MAXSTRING];
    char line[MAXSTRING];
    int v = 0;

    while (v < vertices) {
        bool isWord = false;
        strcpy(urlFile, c[v]);
        strcpy(urlFile + strlen(urlFile), ".txt");
        if ((fp = fopen(urlFile, "r")) != NULL) {
            while (fgets(line, MAXSTRING, fp) != NULL) {
                line[strlen(line) - 1] = '\0';
                if (strncmp(line, "#start Section-2", 16) == 0) {
                    isWord = true;
                    continue;
                }
                else if (strncmp(line, "#end Section-2", 14) == 0) {
                    break;
                }
                if (isWord) {
                    token = strtok(line, delim);
                    while (token != NULL) {
                        for (int i = 0; i < strlen(token); i++) {
                            token[i] = tolower(token[i]);
                        }
                        insertSetUrls(list, token, c[v]);
                        token = strtok(NULL, delim);
                    }
                }
            }
            fclose(fp);
        }
        v++;
    }
}

int main() {
    // get the total amount of the urls
    char buff[MAXSTRING];
    int vertices = 0;

    FILE *fp = fopen("collection.txt", "r");
    while (fscanf(fp, "%s", buff) != EOF) {
        vertices++;
    }
    fclose(fp);

    // generate a collection array
    char *c[vertices];

    FILE *fp2 = fopen("collection.txt", "r");
    for (int i = 0; i < vertices; i++) {
        c[i] = malloc(MAXSTRING * sizeof(char));
        fscanf(fp2, "%s", c[i]);
    }
    fclose(fp2);

    DLListStr list = newDLListStr();
    scanS2(c, vertices, list);

    sortDLListStr(list);
    printf("\nSorted list.\n");
    showDLListStr(list);

    // generate the invertedIndex.txt
    FILE *fp3;
    if ((fp3 = fopen("invertedIndex.txt", "w")) != NULL) {
        copyDLListStr(list, fp3);
    }
    fclose(fp3);

    // free
    freeDLListStr(list);
    for (int i = 0; i < vertices; i++) {
        free(c[i]);
    }
    free(c);

    return 0;
}