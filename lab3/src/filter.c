#include "filter.h"

void free_image(image_t image) {
  free_img((void**) image.img);
}
void free_image_3(image3_t image) {
  for (int ch = 0; ch < 3; ch++) free_img((void**) image.img[ch]);
}

void apply_filter(image_t input, image_t filter, image_t output) {
  // Applies filter to img all on tpre-allocated memory
  // Pretty inefficient approach with a bunch of for-loops
  assert(input.height == output.height);
  assert(input.width == output.width);
  assert(filter.height % 2);
  assert(filter.width % 2);

  int32_t shift_h = (filter.height - 1) / 2;
  int32_t shift_w = (filter.width - 1) / 2;

  for (int32_t i = 0; i < input.height; i++) 
  for (int32_t j = 0; j < input.width; j++) {
    output.img[i][j] = 0.0; 
    // fprintf ( stderr, "sdfdf %i, %i\n", i, j );
    for (int32_t ii = 0; ii < filter.height; ii++)
    for (int32_t jj = 0; jj < filter.width; jj++) {
      int idx_im_y = i - ii + shift_h;
      int idx_im_x = j - jj + shift_w;
      int idx_f_y = ii;
      int idx_f_x = jj;
      if (0 <= idx_im_y && idx_im_y < input.height &&
      0 <= idx_im_x && idx_im_x < input.width &&
      0 <= idx_f_y && idx_f_y < filter.height &&
      0 <= idx_f_x && idx_f_x < filter.width) {
        output.img[i][j] += input.img[idx_im_y][idx_im_x] * 
                          filter.img[idx_f_y][idx_f_x];
      } 
    }
  }
}

void apply_filter_3(image3_t input, image3_t filter, image3_t output) {
  for (int ch = 0; ch < 3; ch++) {
    image_t channel_img = {input.height, input.width, input.img[ch]};
    image_t channel_filter = {filter.height, filter.width, filter.img[ch]};
    image_t channel_output = {output.height, output.width, output.img[ch]};
    apply_filter(channel_img, channel_filter, channel_output);
  }
}

void open_routine(FILE *fp, char* filename, struct TIFF_img* read_image, char type_check) {
  /* open image file */
  if ( ( fp = fopen ( filename, "rb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file %s\n", filename );
    exit ( 1 );
  }

  /* read image */
  if ( read_TIFF ( fp, read_image ) ) {
    fprintf ( stderr, "error reading file %s\n", filename );
    exit ( 1 );
  }

  /* close image file */
  fclose ( fp );

  /* check the type of image data */
  if ( read_image->TIFF_type != type_check ) {
    fprintf ( stderr, "WARNING:  Wrong type: %c\n", read_image->TIFF_type);
  }
}

void write_routine(FILE *fp, char* filename, struct TIFF_img* write_image) {
    /* open color image file */
  if ( ( fp = fopen (filename, "wb" ) ) == NULL ) {
      fprintf ( stderr, "cannot open file %sf\n", filename);
      exit ( 1 );
  }
    
  /* write color image */
  if ( write_TIFF ( fp, write_image ) ) {
      fprintf ( stderr, "error writing TIFF file %s\n", filename );
      exit ( 1 );
  }
    
  /* close color image file */
  fclose ( fp );
}

void populate_tiff_from_img3(image3_t* img, struct TIFF_img* color_img) {
  int32_t i,j,pixel;

  for ( i = 0; i < img->height; i++ )
  for ( j = 0; j < img->width; j++ ) 
  for (int ch = 0; ch < 3; ch++) {
    pixel = (int32_t)img->img[ch][i][j];
    if(pixel>255) {
      color_img->color[ch][i][j] = 255;
    }
    else {
      if(pixel<0) color_img->color[ch][i][j] = 0;
      else color_img->color[ch][i][j] = pixel; 
    }
  }
}


void populate_tiff_from_img(image_t* img, struct TIFF_img* mono_img) {
  int32_t i,j,pixel;

  for ( i = 0; i < img->height; i++ )
  for ( j = 0; j < img->width; j++ ) {
    pixel = (int32_t)img->img[i][j];
    if(pixel>255) {
      mono_img->mono[i][j] = 255;
    }
    else {
      if(pixel<0) mono_img->mono[i][j] = 0;
      else mono_img->mono[i][j] = pixel; 
    }
  }
}

void allocate_img3(image3_t* target_img, int height, int width) {
  target_img->height =  height;
  target_img->width =  width;
  for (int ch = 0; ch < 3; ch++) {
    target_img->img[ch] = (double **)get_img(width,height,sizeof(double));
  }
}

void allocate_img(image_t* target_img, int height, int width) {
  target_img->height =  height;
  target_img->width =  width;
  target_img->img = (double **)get_img(width,height,sizeof(double));
}

