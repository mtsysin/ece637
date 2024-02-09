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
  double** img[3];
} image3_t;

void free_image(image_t image);
void free_image_3(image3_t image);

void open_routine(FILE *fp, char* filename, struct TIFF_img* read_image, char type_check);
void write_routine(FILE *fp, char* filename, struct TIFF_img* write_image);
void allocate_img(image_t* target_img, int height, int width);
void allocate_img3(image3_t* target_img, int height, int width);
void populate_tiff_from_img(image_t* img, struct TIFF_img* mono_img);
void populate_tiff_from_img3(image3_t* img, struct TIFF_img* color_img);


void apply_filter(image_t input, image_t filter, image_t output);
void apply_filter_3(image3_t input, image3_t filter, image3_t output);

#endif //_FILTER_H_
