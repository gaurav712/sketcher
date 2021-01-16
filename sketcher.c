#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include <SDL2/SDL.h>

static float convolution_kernel[3][3] = {
    { 0.0275, 0.1102, 0.0275 },
    { 0.1102, 0.4421, 0.1102 },
    { 0.0275, 0.1102, 0.0275 }
};

static int sobel_kernel_v[3][3] = {
    { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 }
};

static int sobel_kernel_h[3][3] = {
    { -1, -2, -1 },
    {  0,  0,  0 },
    {  1,  2,  1 }
};

int main(int argc, char *argv[]) {

    int width, height;
    int **grayscale_data, **sketch_data;

    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;

    // the pixel array
    png_bytep *rows;

    // validate command line arguments
    if (argc < 2) {
        fprintf(stderr, "Please input filename!\n");
        exit(1);
    }

    // load the input file
    fp = fopen(argv[1], "r");

    // initialize libpng
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);

    printf("image is %dx%d\n", width, height);

    // allocate memory for rows
    rows = (png_bytep *) malloc(sizeof(png_bytep) * height);

    // allocate memory for each row
    for(int row_count = 0; row_count < height; row_count++) {
        rows[row_count] = (png_byte *) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    // read row data from png
    png_read_image(png_ptr, rows);

    // allocate memory for grayscale image
    grayscale_data = (int **) malloc(sizeof(int *) * height);

    // for each pixel now
    for (int row_count = 0; row_count < height; row_count++)
        grayscale_data[row_count] = (int *) malloc(sizeof(int) * width);

    // convert image to grayscale for better edge detection
    for (int row_count = 0; row_count < height; row_count++) {
        for (int pixel_count = 0; pixel_count < width; pixel_count++) {
            grayscale_data[row_count][pixel_count] = (
                    rows[row_count][pixel_count * 4] +
                    rows[row_count][pixel_count * 4 + 1] +
                    rows[row_count][pixel_count * 4 + 2]
                    ) / 3;
        }
    }

    // free rows
    for(int row_count = 0; row_count < height; row_count++) {
        free(rows[row_count]);
    }
    free(rows);

    // apply gaussian blur

    // for now, I am ignoring the 1 px wide edge to avoid weird conditions
    for (int row_count = 1; row_count < (height - 1); row_count++) {
        for (int pixel_count = 1; pixel_count < (width - 1); pixel_count++) {

            grayscale_data[row_count][pixel_count] =
                grayscale_data[row_count - 1][pixel_count - 1] * convolution_kernel[0][0] +
                grayscale_data[row_count - 1][pixel_count] * convolution_kernel[0][1] +
                grayscale_data[row_count - 1][pixel_count + 1] * convolution_kernel[0][2] +

                grayscale_data[row_count][pixel_count - 1] * convolution_kernel[1][0] +
                grayscale_data[row_count][pixel_count] * convolution_kernel[1][1] +
                grayscale_data[row_count][pixel_count + 1] * convolution_kernel[1][2] +

                grayscale_data[row_count + 1][pixel_count - 1] * convolution_kernel[2][0] +
                grayscale_data[row_count + 1][pixel_count] * convolution_kernel[2][1] +
                grayscale_data[row_count + 1][pixel_count + 1] * convolution_kernel[2][2];
        }
    }

    // allocate memory for sketch
    sketch_data = (int **) malloc(sizeof(int *) * height);

    // for each pixel now
    for (int row_count = 0; row_count < height; row_count++)
        sketch_data[row_count] = (int *) malloc(sizeof(int) * width);

    // apply sobel edge detection to find the edges

    for (int row_count = 1; row_count < (height - 1); row_count++) {
        for (int pixel_count = 1; pixel_count < (width - 1); pixel_count++) {

            sketch_data[row_count][pixel_count] = sqrt(
                pow(
                    grayscale_data[row_count - 1][pixel_count - 1] * sobel_kernel_v[0][0] +
                    grayscale_data[row_count - 1][pixel_count] * sobel_kernel_v[0][1] +
                    grayscale_data[row_count - 1][pixel_count + 1] * sobel_kernel_v[0][2] +

                    grayscale_data[row_count][pixel_count - 1] * sobel_kernel_v[1][0] +
                    grayscale_data[row_count][pixel_count] * sobel_kernel_v[1][1] +
                    grayscale_data[row_count][pixel_count + 1] * sobel_kernel_v[1][2] +
                    
                    grayscale_data[row_count + 1][pixel_count - 1] * sobel_kernel_v[2][0] +
                    grayscale_data[row_count + 1][pixel_count] * sobel_kernel_v[2][1] +
                    grayscale_data[row_count + 1][pixel_count + 1] * sobel_kernel_v[2][2]
                , 2) +
                pow(
                    grayscale_data[row_count - 1][pixel_count - 1] * sobel_kernel_h[0][0] +
                    grayscale_data[row_count - 1][pixel_count] * sobel_kernel_h[0][1] +
                    grayscale_data[row_count - 1][pixel_count + 1] * sobel_kernel_h[0][2] +

                    grayscale_data[row_count][pixel_count - 1] * sobel_kernel_h[1][0] +
                    grayscale_data[row_count][pixel_count] * sobel_kernel_h[1][1] +
                    grayscale_data[row_count][pixel_count + 1] * sobel_kernel_h[1][2] +
                    
                    grayscale_data[row_count + 1][pixel_count - 1] * sobel_kernel_h[2][0] +
                    grayscale_data[row_count + 1][pixel_count] * sobel_kernel_h[2][1] +
                    grayscale_data[row_count + 1][pixel_count + 1] * sobel_kernel_h[2][2]
                , 2)
            );
        }
    }

    // initialize SDL
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);

    // create window
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    // draw individual pixels
    for (int row_count = 0; row_count < height; row_count++) {
        for (int pixel_count = 0; pixel_count < width; pixel_count++) {
            SDL_SetRenderDrawColor(
                    renderer,
                    sketch_data[row_count][pixel_count],
                    sketch_data[row_count][pixel_count],
                    sketch_data[row_count][pixel_count],
                    1
                    );
            SDL_RenderDrawPoint(renderer, pixel_count, row_count);
        }
    }

    // make it all visible
    SDL_RenderPresent(renderer);

    // To make the output stay
    SDL_Event event;
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // free grayscale_data
    for(int row_count = 0; row_count < height; row_count++) {
        free(grayscale_data[row_count]);
    }
    free(grayscale_data);

    // free sketch_data
    for(int row_count = 0; row_count < height; row_count++) {
        free(sketch_data[row_count]);
    }
    free(sketch_data);

    // exit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // close the file
    fclose(fp);

    return 0;
}

