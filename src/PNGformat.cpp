#include "PNGformat.h"

#include <png.h>
#include <jpeglib.h>

#include<stdexcept>
#include<string>
#include<vector>
#include<iostream>
#include <stdio.h>
#include <setjmp.h>

using namespace std;

void write_PNG(std::vector<unsigned char> &data, unsigned long long width, unsigned long long height, int bpp, std::string &filename){

    png_bytep row_pointers[height];
    for(int i=0;i<height;i++){
        row_pointers[i]=data.data()+width*4*i;
    }

    FILE *file = fopen(filename.c_str(), "wb");
    if(!file)
        throw invalid_argument("Can't access file: "+filename);

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
}




am::Image read_PNG(string filename) {
    am::Image image;

    png_byte color_type;
    png_bytep *row_pointers = NULL;


    FILE *fp = fopen(filename.c_str(), "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    image.width      = png_get_image_width(png, info);
    image.height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    image.bpp = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(image.bpp == 16)
    png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && image.bpp < 8)
    png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    if (row_pointers) abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * image.height);
    for(int y = 0; y < image.height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    image.rgba32.resize(image.width*image.height*4,0);
    for(int y = 0; y < image.height; y++) {
        png_bytep row = row_pointers[y];
        for(int x = 0; x < image.width; x++) {
            png_bytep px = &(row[x * 4]);
            image.rgba32[(y*image.width+x)*4]=px[0];
            image.rgba32[(y*image.width+x)*4+1]=px[1];
            image.rgba32[(y*image.width+x)*4+2]=px[2];
            image.rgba32[(y*image.width+x)*4+3]=px[3];
        }
    }

    fclose(fp);
    free(row_pointers);

    png_destroy_read_struct(&png, &info, NULL);
    return image;
}












struct my_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */

  jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{

  my_error_ptr myerr = (my_error_ptr)cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
}

METHODDEF(int) do_read_JPEG_file(am::Image &image,struct jpeg_decompress_struct *cinfo, vector<unsigned char> &from);
GLOBAL(int)
read_JPEG_file(am::Image &image, vector<unsigned char> &from)
{
  struct jpeg_decompress_struct cinfo;
  return do_read_JPEG_file(image, &cinfo, from);
}


METHODDEF(int)
do_read_JPEG_file(am::Image &image,struct jpeg_decompress_struct *cinfo, vector<unsigned char> &from)
{
    struct my_error_mgr jerr;

    //FILE *infile;                 /* source file */
    JSAMPARRAY buffer;            /* Output row buffer */
    int row_stride;               /* physical row width in output buffer */


//    if ((infile = fopen(filename, "rb")) == NULL) {
//        fprintf(stderr, "can't open %s\n", filename);
//        return 0;
//    }

    cinfo->err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(cinfo);
        //fclose(infile);
        return 0;
    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_mem_src(cinfo, from.data(),from.size());



    (void)jpeg_read_header(cinfo, TRUE);


    (void)jpeg_start_decompress(cinfo);

    row_stride = cinfo->output_width * cinfo->output_components;

    buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr)cinfo, JPOOL_IMAGE, row_stride, 1);

    int lines=0;
    while (cinfo->output_scanline < cinfo->output_height) {
        (void)jpeg_read_scanlines(cinfo, buffer, 1);
        image.rgba32.insert(image.rgba32.end(),buffer[0],buffer[0]+row_stride);
        lines++;
    }
    image.width=row_stride;
    image.height=lines;

    (void)jpeg_finish_decompress(cinfo);

    jpeg_destroy_decompress(cinfo);

    //fclose(infile);

    return 1;
}


am::Image load_JPEG(std::vector<unsigned char> &from){
    am::Image img;
    read_JPEG_file(img,from);
    return img;
}
