#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <math.h>
#include <png.h>

extern float convolution_kernel[3][3];
extern int sobel_kernel_h[3][3], sobel_kernel_v[3][3];

void blur(int **grayscale_image_data, int image_height, int image_width);
void convert_to_grayscale(int **grayscale_data, png_bytep *rows, int height,
                          int width);
void sobel_edge_detection(int **sketch_data, int **grayscale_data, int height,
                          int width);
#endif // IMAGE_OPERATIONS_H
