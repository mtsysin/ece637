#ifndef _FILTER_H_
#define _FILTER_H_

#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include <assert.h>

typedef struct {
  int32_t			height;
  int32_t			width;
  double**          img;
} image_t;

typedef struct {
  int32_t			height;
  int32_t			width;
  double**          img1;
  double**          img2;
  double**          img3;
} image3_t;

void free_image(image_t image);
void free_image_3(image3_t image);

void error(char *name);

void apply_filter(image_t input, image_t filter, image_t output);
void apply_filter_3(image3_t input, image3_t filter, image3_t output);

#endif