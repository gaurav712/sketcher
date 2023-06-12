#include "image_operations.h"

void blur(int **grayscale_image_data, int image_height, int image_width) {

  /* Ignore the 1 px along the edges */
  for (int row_count = 1; row_count < (image_height - 1); row_count++) {
    for (int pixel_count = 1; pixel_count < (image_width - 1); pixel_count++) {

      grayscale_image_data[row_count][pixel_count] =
          grayscale_image_data[row_count - 1][pixel_count - 1] *
              convolution_kernel[0][0] +
          grayscale_image_data[row_count - 1][pixel_count] *
              convolution_kernel[0][1] +
          grayscale_image_data[row_count - 1][pixel_count + 1] *
              convolution_kernel[0][2] +

          grayscale_image_data[row_count][pixel_count - 1] *
              convolution_kernel[1][0] +
          grayscale_image_data[row_count][pixel_count] *
              convolution_kernel[1][1] +
          grayscale_image_data[row_count][pixel_count + 1] *
              convolution_kernel[1][2] +

          grayscale_image_data[row_count + 1][pixel_count - 1] *
              convolution_kernel[2][0] +
          grayscale_image_data[row_count + 1][pixel_count] *
              convolution_kernel[2][1] +
          grayscale_image_data[row_count + 1][pixel_count + 1] *
              convolution_kernel[2][2];
    }
  }
}

/* Convert pixel data to grayscale */
void convert_to_grayscale(int **grayscale_data, png_bytep *rows, int height,
                          int width) {
  for (int row_count = 0; row_count < height; row_count++) {
    for (int pixel_count = 0; pixel_count < width; pixel_count++) {
      grayscale_data[row_count][pixel_count] =
          (rows[row_count][pixel_count * 4] +
           rows[row_count][pixel_count * 4 + 1] +
           rows[row_count][pixel_count * 4 + 2]) /
          3;
    }
  }
}

/* Apply Sobel edge detection on pixel data */
void sobel_edge_detection(int **sketch_data, int **grayscale_data, int height,
                          int width) {
  for (int row_count = 1; row_count < (height - 1); row_count++) {
    for (int pixel_count = 1; pixel_count < (width - 1); pixel_count++) {

      sketch_data[row_count][pixel_count] =
          sqrt(pow(grayscale_data[row_count - 1][pixel_count - 1] *
                           sobel_kernel_v[0][0] +
                       grayscale_data[row_count - 1][pixel_count] *
                           sobel_kernel_v[0][1] +
                       grayscale_data[row_count - 1][pixel_count + 1] *
                           sobel_kernel_v[0][2] +

                       grayscale_data[row_count][pixel_count - 1] *
                           sobel_kernel_v[1][0] +
                       grayscale_data[row_count][pixel_count] *
                           sobel_kernel_v[1][1] +
                       grayscale_data[row_count][pixel_count + 1] *
                           sobel_kernel_v[1][2] +

                       grayscale_data[row_count + 1][pixel_count - 1] *
                           sobel_kernel_v[2][0] +
                       grayscale_data[row_count + 1][pixel_count] *
                           sobel_kernel_v[2][1] +
                       grayscale_data[row_count + 1][pixel_count + 1] *
                           sobel_kernel_v[2][2],
                   2) +
               pow(grayscale_data[row_count - 1][pixel_count - 1] *
                           sobel_kernel_h[0][0] +
                       grayscale_data[row_count - 1][pixel_count] *
                           sobel_kernel_h[0][1] +
                       grayscale_data[row_count - 1][pixel_count + 1] *
                           sobel_kernel_h[0][2] +

                       grayscale_data[row_count][pixel_count - 1] *
                           sobel_kernel_h[1][0] +
                       grayscale_data[row_count][pixel_count] *
                           sobel_kernel_h[1][1] +
                       grayscale_data[row_count][pixel_count + 1] *
                           sobel_kernel_h[1][2] +

                       grayscale_data[row_count + 1][pixel_count - 1] *
                           sobel_kernel_h[2][0] +
                       grayscale_data[row_count + 1][pixel_count] *
                           sobel_kernel_h[2][1] +
                       grayscale_data[row_count + 1][pixel_count + 1] *
                           sobel_kernel_h[2][2],
                   2));
    }
  }
}
