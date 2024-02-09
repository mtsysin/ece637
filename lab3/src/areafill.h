#ifndef _AREAFILL_H_
#define _AREAFILL_H_

#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include <assert.h>

struct pixel{
    int m, n;
};

typedef struct pixel pixel_t;

struct Node{
    pixel_t p;
    struct Node* next;
};

typedef struct Node node;

void ConnectedNeighbors(
    struct pixel s,
    double T,
    unsigned char **img,
    int width,
    int height,
    int *M,
    struct pixel c[4]
);

void ConnectedSet(
    struct pixel s,
    double T,
    unsigned char **img,
    int width,
    int height,
    int ClassLabel,
    unsigned int **seg,
    int *NumConPixels
);

void Segment(
    double T,
    unsigned char **img,
    int width,
    int height,
    unsigned int **seg
);

#endif //_AREAFILL_H_
