/*
 * Copyright (c) 2023 Gaurav Kumar Yadav
 * Licensed under the MIT License
 */

#include "main.h"

int main(int argc, char *argv[]) {

  int width, height;
  int **grayscale_data, **sketch_data;

  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;

  /* The pixel grid */
  png_bytep *rows;

  /* Validate command line arguments */
  if (argc < 2) {
    show_usage(argv[1]);
    exit(1);
  }

  /* Load the input file */
  fp = fopen(argv[1], "r");

  /* initialize libpng */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, fp);

  png_read_info(png_ptr, info_ptr);

  /* Get height and weight of the image */
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);

  /* Allocate memory for pixel data from the image */
  rows = (png_bytep *)malloc(sizeof(png_bytep) * height);

  for (int row_count = 0; row_count < height; row_count++) {
    rows[row_count] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
  }

  /* Get pixel data from the image */
  png_read_image(png_ptr, rows);

  /* Allocate memory for grayscale pixel data */
  grayscale_data = (int **)malloc(sizeof(int *) * height);

  for (int row_count = 0; row_count < height; row_count++)
    grayscale_data[row_count] = (int *)malloc(sizeof(int) * width);

  /* Convert image to grayscale for better edge detection */
  convert_to_grayscale(grayscale_data, rows, height, width);

  /* Free memory for pixel data */
  for (int row_count = 0; row_count < height; row_count++) {
    free(rows[row_count]);
  }
  free(rows);

  /* Apply gaussian blur */
  for (int blur_cycle = 0; blur_cycle < BLUR_CYCLES; blur_cycle++)
    blur(grayscale_data, height, width);

  /* Allocate memory for sketch */
  sketch_data = (int **)malloc(sizeof(int *) * height);

  for (int row_count = 0; row_count < height; row_count++)
    sketch_data[row_count] = (int *)malloc(sizeof(int) * width);

  /* Apply sobel edge detection to find the edges */
  sobel_edge_detection(sketch_data, grayscale_data, height, width);

  /* initialize SDL */
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);

  /* Create window */
  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

  /* Draw individual pixels */
  for (int row_count = 0; row_count < height; row_count++) {
    for (int pixel_count = 0; pixel_count < width; pixel_count++) {
      SDL_SetRenderDrawColor(renderer, sketch_data[row_count][pixel_count],
                             sketch_data[row_count][pixel_count],
                             sketch_data[row_count][pixel_count], 1);
      SDL_RenderDrawPoint(renderer, pixel_count, row_count);
    }
  }

  /* Make it all visible */
  SDL_RenderPresent(renderer);

  /* To make the output stay */
  SDL_Event event;
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }

  /* Free grayscale pixel data */
  for (int row_count = 0; row_count < height; row_count++) {
    free(grayscale_data[row_count]);
  }
  free(grayscale_data);

  /* Free sketch_data */
  for (int row_count = 0; row_count < height; row_count++) {
    free(sketch_data[row_count]);
  }
  free(sketch_data);

  /* Clean up SDL */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  /* Close the file */
  fclose(fp);

  return 0;
}
