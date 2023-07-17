#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c) {
    int ix = (int)roundf(x);
    int iy = (int)roundf(y);
    return get_pixel(im, ix, iy, c);
}

image nn_resize(image im, int w, int h) {
    image resized = make_image(w, h, im.c);
    float scale_x = (float)im.w / w;
    float scale_y = (float)im.h / h;

    for (int c = 0; c < im.c; c++) {
        for (int resized_y = 0; resized_y < h; resized_y++) {
            for (int resized_x = 0; resized_x < w; resized_x++) {
                float original_x = (resized_x + 0.5) * scale_x - 0.5;
                float original_y = (resized_y + 0.5) * scale_y - 0.5;
                set_pixel(resized, resized_x, resized_y, c, nn_interpolate(im, original_x, original_y, c));
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c) {
    int ix = (int)floorf(x);
    int iy = (int)floorf(y);
    float dx = x - ix;
    float dy = y - iy;

    float val1 = (1 - dx) * (1 - dy) * get_pixel(im, ix, iy, c);
    float val2 = dx * (1 - dy) * get_pixel(im, ix + 1, iy, c);
    float val3 = (1 - dx) * dy * get_pixel(im, ix, iy + 1, c);
    float val4 = dx * dy * get_pixel(im, ix + 1, iy + 1, c);

    return val1 + val2 + val3 + val4;
}

image bilinear_resize(image im, int w, int h) {
    image resized = make_image(w, h, im.c);
    float scale_x = (float)im.w / w;
    float scale_y = (float)im.h / h;

    for (int c = 0; c < im.c; c++) {
        for (int resized_y = 0; resized_y < h; resized_y++) {
            for (int resized_x = 0; resized_x < w; resized_x++) {
                float original_x = (resized_x + 0.5) * scale_x - 0.5;
                float original_y = (resized_y + 0.5) * scale_y - 0.5;
                set_pixel(resized, resized_x, resized_y, c, bilinear_interpolate(im, original_x, original_y, c));
            }
        }
    }

    return resized;
}