#include "PNGformat.h"

#include<png.h>
#include<stdexcept>

using namespace std;

int write_PNG(unsigned char* data, long long width, long long height, int bpp, const char* filename){

    png_bytep row_pointers[height];
    for(int i=0;i<height;i++){
        row_pointers[i]=data+width*4*i;
    }

    FILE *file = fopen(filename, "wb");
    if(!file)
        throw invalid_argument("Can't access file.");

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        throw invalid_argument("Can't create PNG struct.");

    png_infop info = png_create_info_struct(png);
    if (!info)
        throw invalid_argument("Can't create INFO struct.");

    if (setjmp(png_jmpbuf(png)))
        throw invalid_argument("PNG.png stopped working.");

    png_init_io(png, file);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
    png,
    info,
    width, height,
    bpp,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    if (!row_pointers)
        throw invalid_argument("Row pointer problem here.");

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

//    for(int y = 0; y < data.height; y++) {
//        free(row_pointers[y]);
//    }
//    free(row_pointers);

    fclose(file);

    png_destroy_write_struct(&png, &info);

    return 0;

}
