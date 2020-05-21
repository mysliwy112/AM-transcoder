#ifndef IMAGE_H
#define IMAGE_H

#include<vector>
#include<string>

#include "am_utils.h"

namespace am{

    enum comp{
        none,
        uknw,
        clzw,
        crzw,
        crle,
        jpeg
    };

    struct image_data{
        bytes header;
        bytes image;
        bytes alpha;
    };

    class Image : public Graphics
    {
        public:
            Image();

            std::string name="magick";

            unsigned int width;
            unsigned int height;

            short position_x=0;
            short position_y=0;

            unsigned int image_size;
            unsigned int alpha_size;
            int compression;
            int bpp=16;

            bytes rgba32;

            void load_ann(bytes::iterator &offset);
            dic load_mann(std::stringstream &offset);

            void load_img(bytes::iterator &offset);
            void load_img(bytes data);

            void log();


            bytes get_ann_header(int compression,int isize,int asize);
            bytes get_img_header(int compression,int isize,int asize);
            image_data get_am_data(int compression);
            image_data get_ann();
            void get_mann(std::ostringstream &offset,std::string &file);

            void load_data(bytes data);
            void load_rgba32(bytes data);
            bytes get_rgba32();

            void read_png(std::string filename);
            void write_png(std::string filename);
            void read_img(std::string filename);
            void write_img(std::string filename);

            bytes decompress(bytes data, int type, int size);
            bytes compress(bytes data, int type, int size);

            void create_rgba32(image_data img);
            image_data split_rgba32();

            void align(int max_x=0, int max_y=0);


        protected:

        private:
    };
}

#endif // IMAGE_H
