#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>
#include <math.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#define BLUR_CYCLES 5 /* Number of times the blur is applied */

float convolution_kernel[3][3] = {{0.0275, 0.1102, 0.0275},
                                  {0.1102, 0.4421, 0.1102},
                                  {0.0275, 0.1102, 0.0275}};

int sobel_kernel_v[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

int sobel_kernel_h[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

#endif // CONFIG_H
