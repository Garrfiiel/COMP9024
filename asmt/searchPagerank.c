//
// Created by wangziyi on 2020/1/31.
//

#include "DLListStr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSTRING 1000

int main(int argc, char *argv[]) {

    if (argc > 1) {

        DLListStr pageRank = newDLListStr();

        char line[MAXSTRING];
        char delim[3] = " ,";
        FILE *fp;
        char *token;
        int a;
        char url[MAXSTRING];
        int outDegree = 0;
        double rankValue = 0.0;

        if ((fp = fopen("pagerankList.txt", "r")) != NULL) {
            while (fgets(line, MAXSTRING, fp) != NULL) {
                line[strlen(line) - 1] = '\0';    // remove \n
                a = 0;
                /* first token */
                token = strtok(line, delim);
                /* iterate over the rest of the tokens */
                while (token != NULL) {
                    if (a == 0) {
                        strcpy(url, token);
                        url[strlen(token)] = '\0';
                    }
                    if (a == 1) {
                        outDegree = atoi(token);
                    }
                    if (a == 2) {
                        rankValue = atof(token);
                    }
                    token = strtok(NULL, delim);
                    a++;
                }
                insertSetPageRank(pageRank, url, outDegree, rankValue);
            }
            fclose(fp);
        }

        DLListStr inverted = newDLListStr();

        FILE *fp2;
        char word[MAXSTRING];

        if ((fp2 = fopen("invertedIndex.txt", "r")) != NULL) {
            while (fgets(line, MAXSTRING, fp2) != NULL) {
                line[strlen(line) - 1] = '\0';
                a = 0;
                /* first token */
                token = strtok(line, delim);
                /* iterate over the rest of the tokens */
                while (token != NULL) {
                    if (a == 0) {
                        strcpy(word, token);
                        word[strlen(token)] = '\0';
                    } else {

                        insertSetUrls(inverted, word, token);
                    }
                    token = strtok(NULL, delim);
                    a++;
                }

            }
            fclose(fp2);
        }

        DLListStr finalRank = newDLListStr();
        // search words
        for(int i = 0; i < argc; i++) {
            searchDLListStr(i, argv, pageRank, inverted, finalRank);
        }
        sortDLListStrPageRank(finalRank);
        showFinalRank(finalRank);

        // free
        freeDLListStr(inverted);
        freeDLListStr(pageRank);
        freeDLListStr(finalRank);
    }
    return 0;
}