#include "filter.h"

void free_image(image_t image) {
  free_img((void**) image.img);
}
void free_image_3(image3_t image) {
  free_img((void**) image.img1);
  free_img((void**) image.img2);
  free_img((void**) image.img3);
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

    for (int32_t i = 0; i < input.height; i++) {
        for (int32_t j = 0; j < input.width; j++) {
          output.img[i][j] = 0.0; 
              fprintf ( stderr, "sdfdf %i, %i\n", i, j );
            for (int32_t ii = 0; ii < filter.height; ii++) {
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
    }
}

void apply_filter_3(image3_t input, image3_t filter, image3_t output) {
    image_t r = {
        input.height, 
        input.width, 
        input.img1
    };
    image_t g = {
        input.height, 
        input.width, 
        input.img2
    };  
    image_t b = {
        input.height, 
        input.width, 
        input.img3
    };
    ///
    image_t fr = {
        filter.height, 
        filter.width, 
        filter.img1
    };
    image_t fg = {
        filter.height, 
        filter.width, 
        filter.img2
    };  
    image_t fb = {
        filter.height, 
        filter.width, 
        filter.img3
    };
    ///    
    image_t or = {
        output.height, 
        output.width, 
        output.img1
    };
    image_t og = {
        output.height, 
        output.width, 
        output.img2
    };  
    image_t ob = {
        output.height, 
        output.width, 
        output.img3
    };
    apply_filter(r, fr, or);
    apply_filter(g, fg, og);
    apply_filter(b, fb, ob);
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
    fprintf ( stderr, "WARNING:  Wrong type: %s\n", read_image->TIFF_type);
    // exit ( 1 );
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
  int32_t i,j,pixel1,pixel2,pixel3;

  for ( i = 0; i < img->height; i++ )
  for ( j = 0; j < img->width; j++ ) {
    pixel1 = (int32_t)img->img1[i][j];
    pixel2 = (int32_t)img->img2[i][j];
    pixel3 = (int32_t)img->img3[i][j];

    if(pixel1>255) {
      color_img->color[0][i][j] = 255;
    }
    else {
      if(pixel1<0) color_img->color[0][i][j] = 0;
      else color_img->color[0][i][j] = pixel1;
    }

    if(pixel2>255) {
      color_img->color[1][i][j] = 255;
    }
    else {
      if(pixel2<0) color_img->color[1][i][j] = 0;
      else color_img->color[1][i][j] = pixel2;
    }

    if(pixel3>255) {
      color_img->color[2][i][j] = 255;
    }
    else {
      if(pixel3<0) color_img->color[2][i][j] = 0;
      else color_img->color[2][i][j] = pixel3;
    }
  }
}

void allocate_img3(image3_t* target_img, int height, int width) {
  target_img->height =  height;
  target_img->width =  width,
  target_img->img1 = (double **)get_img(width,height,sizeof(double));
  target_img->img2 = (double **)get_img(width,height,sizeof(double));
  target_img->img3 = (double **)get_img(width,height,sizeof(double));
}

int main (int argc, char **argv) 
{
  FILE *fp = 0;
  struct TIFF_img input_img_tiff, filter_tiff, output_img_tiff;
  image3_t input_img, filtered_image, sharp_filtered_image, custom_filtered_image, filter, filter_sharp, filter_generated;
  int32_t i,j;
  float lambda;

  if ( argc != 4 ) error( argv[0] );

  open_routine(fp, argv[1], &input_img_tiff, 'c');

  /* Allocate image of double precision floats */

  allocate_img3(&input_img, input_img_tiff.height, input_img_tiff.width);
  allocate_img3(&filtered_image, input_img_tiff.height, input_img_tiff.width);
  allocate_img3(&sharp_filtered_image, input_img_tiff.height, input_img_tiff.width);
  allocate_img3(&custom_filtered_image, input_img_tiff.height, input_img_tiff.width);

  /* copy all components */
  for ( i = 0; i < input_img.height; i++ )
  for ( j = 0; j < input_img.width; j++ ) {
    input_img.img1[i][j] = input_img_tiff.color[0][i][j];
    input_img.img2[i][j] = input_img_tiff.color[1][i][j];
    input_img.img3[i][j] = input_img_tiff.color[2][i][j];
  }

  // Create Filters
  allocate_img3(&filter, 9, 9);

  for ( i = 0; i < filter.height; i++ )
  for ( j = 0; j < filter.width; j++ ) {
    filter.img1[i][j] = 1.0/81.0;
    filter.img2[i][j] = 1.0/81.0;
    filter.img3[i][j] = 1.0/81.0;
  }

  // Create sharp filter

  lambda = atof(argv[2]);
  allocate_img3(&filter_sharp, 5, 5);

  for ( i = 0; i < filter_sharp.height; i++ )
  for ( j = 0; j < filter_sharp.width; j++ ) {
    int delta  = (int)(i==0 && j==0);
    filter_sharp.img1[i][j] = delta + lambda * (delta - 1.25);
    filter_sharp.img2[i][j] = delta + lambda * (delta - 1.25);
    filter_sharp.img3[i][j] = delta + lambda * (delta - 1.25);
  }

  // Create custom filter;
  // NOTE:::
  // In tiff.c certain flag checks were removed (XResolution and YResolution)
  open_routine(fp, argv[3], &filter_tiff, 'g');
  allocate_img3(&filter_generated, filter_tiff.height, filter_tiff.width);

  for ( i = 0; i < filter_generated.height; i++ )
  for ( j = 0; j < filter_generated.width; j++ ) {
    filter_generated.img1[i][j] = filter_tiff.mono[i][j];
    filter_generated.img2[i][j] = filter_tiff.mono[i][j];
    filter_generated.img3[i][j] = filter_tiff.mono[i][j];
  }
  /* Filter image  */
  apply_filter_3(input_img, filter, filtered_image);
  apply_filter_3(input_img, filter_sharp, sharp_filtered_image);
  apply_filter_3(input_img, filter_generated, custom_filtered_image);
    
  /* set up structure for output color image */
  /* Note that the type is 'c' rather than 'g' */
  get_TIFF ( &output_img_tiff, input_img.height, input_img.width, 'c');

  //Save all the images
  populate_tiff_from_img3(&filtered_image, &output_img_tiff);
  write_routine(fp, "filtered.tif", &output_img_tiff);

  populate_tiff_from_img3(&sharp_filtered_image, &output_img_tiff);
  write_routine(fp, "sharp_filtered.tif", &output_img_tiff);

  populate_tiff_from_img3(&custom_filtered_image, &output_img_tiff);
  write_routine(fp, "custom_filtered.tif", &output_img_tiff);

  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img_tiff) );
  free_TIFF ( &(output_img_tiff) );
  free_TIFF ( &(filter_tiff) );
  
  free_image_3(input_img);
  free_image_3(filtered_image);  
  free_image_3(sharp_filtered_image);  
  free_image_3(custom_filtered_image);  
  free_image_3(filter);  
  free_image_3(filter_sharp);  
  free_image_3(filter_generated);  

  return(0);
}

void error(char *name)
{
    printf("usage:  %s  input.tiff lambda_value filter.tiff \n\n",name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("and a customfilter image.\n");
    printf("It then performs all tasks required in the lab\n");
    exit(1);
}
