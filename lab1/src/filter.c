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


int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, color_img, color_img_sharp;
//   double **img1,**img2;
  image3_t img1, img2, img3, filter, filter_sharp;
  int32_t i,j,pixel1,pixel2,pixel3;
  float lambda;

  if ( argc != 3 ) error( argv[0] );

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file %s\n", argv[1] );
    exit ( 1 );
  }

  /* read image */
  if ( read_TIFF ( fp, &input_img ) ) {
    fprintf ( stderr, "error reading file %s\n", argv[1] );
    exit ( 1 );
  }

  /* close image file */
  fclose ( fp );

  /* check the type of image data */
  if ( input_img.TIFF_type != 'c' ) {
    fprintf ( stderr, "error:  image must be 24-bit color\n" );
    exit ( 1 );
  }

  /* Allocate image of double precision floats */

  img1.height = input_img.height;
  img1.width =  input_img.width,
  img1.img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img1.img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img1.img3 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  img2.height = input_img.height;
  img2.width =  input_img.width,
  img2.img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img2.img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img2.img3 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  img3.height = input_img.height;
  img3.width =  input_img.width,
  img3.img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img3.img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  img3.img3 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  /* copy all components to double array */
  for ( i = 0; i < img1.height; i++ )
  for ( j = 0; j < img1.width; j++ ) {
    img1.img1[i][j] = input_img.color[0][i][j];
    img1.img2[i][j] = input_img.color[1][i][j];
    img1.img3[i][j] = input_img.color[2][i][j];
  }

  // Create Filter
  filter.height = 9;
  filter.width =  9;
  filter.img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filter.img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filter.img3 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  for ( i = 0; i < filter.height; i++ )
  for ( j = 0; j < filter.width; j++ ) {
    filter.img1[i][j] = 1.0/81.0;
    filter.img2[i][j] = 1.0/81.0;
    filter.img3[i][j] = 1.0/81.0;
  }

  // Create sharp filter
    // Create Filter

  lambda = 1.5;
  filter_sharp.height = 5;
  filter_sharp.width =  5;
  filter_sharp.img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filter_sharp.img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filter_sharp.img3 = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  for ( i = 0; i < filter_sharp.height; i++ )
  for ( j = 0; j < filter_sharp.width; j++ ) {
    filter.img1[i][j] = (int)(i==j) + lambda * ((int)(i==j) - 1.25);
    filter.img2[i][j] = (int)(i==j) + lambda * ((int)(i==j) - 1.25);
    filter.img3[i][j] = (int)(i==j) + lambda * ((int)(i==j) - 1.25);
  }

  /* Filter image  */
  apply_filter_3(img1, filter, img2);
    
  /* set up structure for output color image */
  /* Note that the type is 'c' rather than 'g' */
  get_TIFF ( &color_img, input_img.height, input_img.width, 'c' );

  /* copy green component to new images */
  for ( i = 0; i < input_img.height; i++ )
  for ( j = 0; j < input_img.width; j++ ) {
    pixel1 = (int32_t)img2.img1[i][j];
    pixel2 = (int32_t)img2.img2[i][j];
    pixel3 = (int32_t)img2.img3[i][j];

    if(pixel1>255) {
      color_img.color[0][i][j] = 255;
    }
    else {
      if(pixel1<0) color_img.color[0][i][j] = 0;
      else color_img.color[0][i][j] = pixel1;
    }

    if(pixel2>255) {
      color_img.color[1][i][j] = 255;
    }
    else {
      if(pixel2<0) color_img.color[1][i][j] = 0;
      else color_img.color[1][i][j] = pixel2;
    }

    if(pixel3>255) {
      color_img.color[2][i][j] = 255;
    }
    else {
      if(pixel3<0) color_img.color[2][i][j] = 0;
      else color_img.color[2][i][j] = pixel3;
    }
  }
    
    
  /* open color image file */
  if ( ( fp = fopen ( "color.tif", "wb" ) ) == NULL ) {
      fprintf ( stderr, "cannot open file color.tif\n");
      exit ( 1 );
  }
    
  /* write color image */
  if ( write_TIFF ( fp, &color_img ) ) {
      fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
      exit ( 1 );
  }
    
  /* close color image file */
  fclose ( fp );

  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img) );
  free_TIFF ( &(color_img) );
  
  free_image_3(img1);
  free_image_3(img2);  

  return(0);
}

void error(char *name)
{
    printf("usage:  %s  image.tiff \n\n",name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("It then horizontally filters the green component, adds noise,\n");
    printf("and writes out the result as an 8-bit image\n");
    printf("with the name 'green.tiff'.\n");
    printf("It also generates an 8-bit color image,\n");
    printf("that swaps red and green components from the input image");
    exit(1);
}
