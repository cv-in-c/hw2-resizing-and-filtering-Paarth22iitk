#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c){
        if(x<0) x=0;
        if(y<0) y=0;
        if(c<0) c=0;
        if(x>=im.w) x=im.w-1;
        if(y>=im.h) y=im.h-1;
        if(c>=im.c) c=im.c-1;
    }
    int index = c * im.h * im.w + y * im.w + x;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c)
    return;
    int index = c * im.h * im.w + y * im.w + x;
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                float value = get_pixel(im,x,y,c);
                set_pixel(copy,x,y,c,value);
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            float R = get_pixel(im,x,y,0);
            float G = get_pixel(im,x,y,1);
            float B = get_pixel(im,x,y,2);
            float Y = 0.299*R + 0.587*G + .114*B;
            set_pixel(gray,x,y,0,Y);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            float pixel = get_pixel(im,x,y,c);
            set_pixel(im,x,y,c,pixel+v);
        }
    }
}

void clamp_image(image im)
{
    for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                float pixel = get_pixel(im,x,y,c);
                if(pixel<0) pixel=0;
                if(pixel>1) pixel=1;
                set_pixel(im,x,y,c,pixel);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            float R=get_pixel(im,x,y,0);
            float G=get_pixel(im,x,y,1);
            float B=get_pixel(im,x,y,2);
            float V = three_way_max(R,G,B);
            float m,c,S;
            m = three_way_min(R,G,B);
            c = V-m;
            if(V!=0) S = c/V;
            else S=0;
            float h1,H;
            if(c!=0){
                if(V==R) h1=(G-B)/c;
                if(V==G) h1=(B-R)/c + 2;
                if(V==B) h1=(R-G)/c + 4;
                if(h1<0) H = h1/6 +1;
                else H = h1/6;
            }
            else H=0;
            set_pixel(im,x,y,0,H);
            set_pixel(im,x,y,1,S);
            set_pixel(im,x,y,2,V);
        }
    }
}


void hsv_to_rgb(image im)
{
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float H = get_pixel(im, x, y, 0);
            float S = get_pixel(im, x, y, 1);
            float V = get_pixel(im, x, y, 2);
            float C = S * V;
            float m = V - C;
            float h1 = H*6;
            float X = C * (1 - fabs(fmod(h1, 2) - 1));
            float R, G, B;
            if (h1 >= 0 && h1 < 1) {
                R = C;
                G = X;
                B = 0;
            } 
            else if (h1 >= 1 && h1 < 2) {
                R = X;
                G = C;
                B = 0;
            } 
            else if (h1 >= 2 && h1 < 3) {
                R = 0;
                G = C;
                B = X;
            } 
            else if (h1 >= 3 && h1 < 4) {
                R = 0;
                G = X;
                B = C;
            } 
            else if (h1 >= 4 && h1 < 5) {
                R = X;
                G = 0;
                B = C;
            } 
            else {
                R = C;
                G = 0;
                B = X;
            }

            set_pixel(im, x, y, 0, R + m);
            set_pixel(im, x, y, 1, G + m);
            set_pixel(im, x, y, 2, B + m);
        }
    }
}
