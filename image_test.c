#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {

    int width, height;

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
                    rows[row_count][pixel_count * 4],
                    rows[row_count][pixel_count * 4 + 1],
                    rows[row_count][pixel_count * 4 + 2],
                    rows[row_count][pixel_count * 4 + 3]
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

    // exit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // close the file
    fclose(fp);

    return 0;
}

