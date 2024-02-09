#include "areafill.h"


void ConnectedNeighbors(
    struct pixel s,
    double T,
    unsigned char **img,
    int width,
    int height,
    int *M,
    struct pixel c[4]
) {    
    // Find all the connected neighbours of a pixel s and return them to the array c
    *M = 0;
    if (s.m > 0 && abs((int)img[s.m][s.n] - (int)img[s.m-1][s.n]) <= T) {
        c[(*M)++] = (struct pixel){s.m-1, s.n};
    }

    if (s.m + 1 < height && abs((int)img[s.m][s.n] - (int)img[s.m+1][s.n]) <= T) {
        c[(*M)++] = (struct pixel){s.m+1, s.n};
    }

    if (s.n > 0 && abs((int)img[s.m][s.n] - (int)img[s.m][s.n-1]) <= T) {
        c[(*M)++] = (struct pixel){s.m, s.n-1};
    }

    if (s.n + 1 < width && abs((int)img[s.m][s.n] - (int)img[s.m][s.n+1]) <= T) {
        c[(*M)++] = (struct pixel){s.m, s.n+1};
    }
}


void ConnectedSet(
    struct pixel s,
    double T,
    unsigned char **img,
    int width,
    int height,
    int ClassLabel,
    unsigned int **seg,
    int *NumConPixels
) {
    *NumConPixels = 0;
    node* b = malloc(sizeof(node));
    b->p = s;
    b->next = NULL;
    node* tail = b;
    while (b != NULL) {
        // Get a pixel form the list;
        pixel_t current_pixel = b->p;
        //get neighbours
        pixel_t c[4];
        int M;
        ConnectedNeighbors(current_pixel, T, img, width, height, &M, c);

        // find all the 
        for (int i = 0 ; i < M; ++i) {
            pixel_t candidate = c[i];
            if (seg[candidate.m][candidate.n]!=ClassLabel) {
                seg[candidate.m][candidate.n]=ClassLabel;
                (*NumConPixels)++;
                node* new_node = malloc(sizeof(node));
                new_node->p = candidate;
                new_node->next = NULL;
                tail->next = new_node;
                tail = new_node;
            }
        }
        node* old_b = b;
        b = b->next;
        free(old_b);
    }
}


void Segment(
    double T,
    unsigned char **img,
    int width,
    int height,
    unsigned int **seg
) {
    int label = 1;
    int NumConPixels;
    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) {
        if (seg[i][j] == 0) {
            ConnectedSet((pixel_t) {i, j}, (double)T, img, 
                width, height, label, seg, &NumConPixels);
            if (NumConPixels >= 100) {
                label ++;
            } else {
                ConnectedSet((pixel_t) {i, j}, (double)T, img,
                    width, height, 0, seg, &NumConPixels);
            }
        }s
    }
}
