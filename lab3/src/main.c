#include "tiff.h"
#include "allocate.h"
#include <assert.h>
#include "filter.h"
#include "areafill.h"

void error(char *name)
{
    printf("usage: %s input.tiff output.tiff pixel_y, pixel_x, T \n\n", name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("and a customfilter image.\n");
    printf("It then performs neighbourhood search\n");
    exit(1);
}

int main (int argc, char **argv) 
{
    FILE *fp = 0;
    struct TIFF_img input_img_tiff, output_img_tiff;
    unsigned char **input_img;
    unsigned int **segmentation;
    int32_t i,j, pixel_y, pixel_x, T, NumConPixels;

    // Parse args:
    if (argc != 6) {
        error(argv[0]);
    }
    pixel_y = atoi(argv[3]);
    pixel_x = atoi(argv[4]);
    T = atoi(argv[5]);

    open_routine(fp, argv[1], &input_img_tiff, 'g');

    /* Allocate image of double precision floats */

    input_img = (unsigned char **)get_img(input_img_tiff.width, input_img_tiff.height, sizeof(unsigned char));
    segmentation = (unsigned int **)get_img(input_img_tiff.width, input_img_tiff.height, sizeof(unsigned int));

    /* copy all components */
    for ( i = 0; i < input_img_tiff.height; i++ )
    for ( j = 0; j < input_img_tiff.width; j++ ) {
    input_img[i][j] = input_img_tiff.mono[i][j];
    segmentation[i][j] = 0;
    }

    // ConnectedSet((pixel_t) {pixel_y, pixel_x}, (double)T, input_img, input_img_tiff.width, input_img_tiff.height, 255, segmentation, &NumConPixels);
    Segment((double)T, input_img, input_img_tiff.width, input_img_tiff.height, segmentation);

    get_TIFF(&output_img_tiff, input_img_tiff.height, input_img_tiff.width, 'g');

    //Save the image
    for ( i = 0; i < input_img_tiff.height; i++ )
    for ( j = 0; j < input_img_tiff.width; j++ ) {
        int pix = segmentation[i][j];
        if(pix>255) {
            output_img_tiff.mono[i][j] = 255;
        }
        else if(pix<0) {
            output_img_tiff.mono[i][j] = 0;
        } else {
            output_img_tiff.mono[i][j] = pix; 
        }
    }

    write_routine(fp, argv[2], &output_img_tiff);

    free_TIFF ( &(input_img_tiff) );
    free_TIFF ( &(output_img_tiff) );

    free_img((void**) input_img);


    printf("Success, exiting...\n");
    return(0);
}
