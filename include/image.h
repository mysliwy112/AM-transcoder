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
        bytes image;
        bytes alpha;
    };

    class Image : public Graphics
    {
        public:
            Image();

            std::string name;

            unsigned int width;
            unsigned int height;

            bool log=true;

            int position_x;
            int position_y;

            unsigned int image_size;
            unsigned int alpha_size;
            int compression;
            int bpp;

            bytes rgba32;

            void load_ann(bytes::iterator &offset);
            bytes get_ann_header(int compression);
            void load_img(bytes::iterator &offset);
            void load_img(bytes data);
            bytes get_img_header(int compression);

            void load_data(bytes data);
            bytes get_am_data();
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

            void align();


        protected:

        private:
    };
}

#endif // IMAGE_H
