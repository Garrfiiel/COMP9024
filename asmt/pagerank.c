#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Graph.h"

#define MAXSTRING 1000

void scan(char **c, Graph g, int vertices) {
    char delim[2] = " ";
    FILE *fp;
    char *token;
    char urlFile[MAXSTRING];
    char line[MAXSTRING];
    bool isUrl = false;
    int v = 0;

    while (v < vertices) {
        strcpy(urlFile, c[v]);
        strcpy(urlFile + strlen(urlFile), ".txt");

        if ((fp = fopen(urlFile, "r")) != NULL) {
            Edge e;
            e.v = v;
            while (fgets(line, MAXSTRING, fp) != NULL) {
                line[strlen(line) - 1] = '\0';
                if (strncmp(line, "#start Section-1", 16) == 0)
                    isUrl = true;
                else if (strncmp(line, "#end Section-1", 14) == 0) {
                    isUrl = false;
                    break;
                }

                if (isUrl) {
                    token = strtok(line, delim);
                    while (token != NULL) {
                        for (int w = 0; w < vertices; w++) {
                            if (strcmp(c[w], token) == 0) {
                                e.w = w;
                                insertEdge(g, e);
                                break;
                            }
                        }
                        token = strtok(NULL, delim);
                    }
                }
            }
            fclose(fp);
        }
        v++;
    }
}

void rank(double d, double diffPR, int maxIterations,
                 Graph g, int vertices, const int *vertexDegree,  double *values) {
    // initialise a matrix
    double *newValues = malloc(vertices * sizeof(double));
    double **matrix = malloc(vertices * sizeof(double));

    for (int i = 0; i < vertices; i++) {
        matrix[i] = malloc(vertices * sizeof(double));
        for (int j = 0; j < vertices; j++) {
            matrix[i][j] = 0.0;
        }
    }

    // algorithm
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (adjacent(g, i, j)) {
                matrix[j][i] = 1.0 / vertexDegree[i];
            }
        }
    }

    int iteration = 0;
    double diff = diffPR;

    while (iteration < maxIterations && diff >= diffPR) {
        diff = 0.0;
        for (int i = 0; i < vertices; i++) {
            double t = 0.0;
            for (int j = 0; j < vertices; j++) {
                t += matrix[i][j] * values[j];
            }
            newValues[i] = (1.0 - d) / vertices + d * t;
        }

        // check equality
        for (int i = 0; i < vertices; i++) {
            diff += fabs(newValues[i] - values[i]);
            values[i] = newValues[i];
        }
        iteration++;
    }

    // free
    free(newValues);
    for (int i = 0; i < vertices; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void sort(int index[], const int *vertexDegree, const double *values, int vertices) {
    for (int i = 1; i < vertices; i++) {
        int e = index[i];
        int j = i - 1;

        // compare
        while (j >= 0 ) {
            int r = 0;
            if (values[index[j]] == values[i]) {
                r = vertexDegree[index[j]] < vertexDegree[i];
            } else {
                r = values[index[j]] < values[i];
            }
            if (r > 0) {
                index[j + 1] = index[j];
                j--;
            } else{
                break;
            }
        }
        index[j + 1] = e;
    }
}

int main(int argc, char *argv[]) {
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

    Graph g = newGraph(vertices);
    scan(c, g, vertices);

    // initialise parameters, rank values and outdegrees
    double d = strtod(argv[1], NULL);
    double diffPR = strtod(argv[2], NULL);
    int maxIterations = atoi(argv[3]);

    double *values = malloc(vertices * sizeof(double));
    for (int i = 0; i < vertices; i++) {
        values[i] = 1.0 / vertices;
    }

    int *vertexDegree = malloc(vertices * sizeof(int));
    for (int l = 0; l < vertices; l++) {
        int outDegree = 0;
        for (int k = 0; k < vertices; k++) {
            if (adjacent(g, l, k)) {
                outDegree++;
            }
        }
        vertexDegree[l] = outDegree;
    }
    rank(d, diffPR, maxIterations, g, vertices, vertexDegree, values);

    // generate a overall index
    int index[vertices];
    for (int i = 0; i < vertices; i++) {
        index[i] = i;
    }
    sort(index, vertexDegree, values, vertices);

    // check the result
    for (int i = 0; i < vertices; i++) {
        printf("%s, %d, %.7f\n", c[index[i]], vertexDegree[index[i]], values[index[i]]);
    }

    // Generate the txt file
    FILE *fp3;
    if ((fp3 = fopen("pagerankList.txt", "w")) != NULL) {
        for (int i = 0; i < vertices; i++) {
            fprintf(fp3, "%s, %d, %.7f\n", c[index[i]], vertexDegree[index[i]], values[index[i]]);
        }
    }
    fclose(fp3);

    // free memory
    freeGraph(g);
    free(values);
    free(vertexDegree);
    for (int i = 0; i < vertices; i++) {
        free(c[i]);
    }
    free(c);

    return 0;
}