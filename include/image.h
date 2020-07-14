#ifndef IMAGE_H
#define IMAGE_H

#include<vector>
#include<string>

#include "am_utils.h"

namespace am{

    enum comp{
        none=0,
        uknw=1,
        clzw=2,
        crzw=3,
        crle=4,
        jpeg=5
    };

    struct image_data{
        bytes header;
        bytes image;
        bytes alpha;
    };

    class Image : public Graphics
    {
        public:
            Image(std::string name="");

            std::string name="";

            std::string mann_dir;

            unsigned int width=0;
            unsigned int height=0;

            short position_x=0;
            short position_y=0;

            unsigned int image_size;
            unsigned int alpha_size;
            int compression=3;
            int bpp=16;

            bytes rgba32;

            void load_ann(bytes::iterator &offset);
            dic load_mann(std::stringstream &offset);

            void load_img(bytes::iterator &offset);
            void load_img(bytes data);

            dic load_mimg(std::stringstream &offset);
            void load_mimg(bytes data);

            void load(bytes data);
            void load_png(bytes data);

            void log();


            bytes get_ann_header(int isize,int asize);
            bytes get_img_header(int isize,int asize);
            image_data get_am_data();
            image_data get_ann(bool doimages);
            void get_mann(std::ostringstream &offset,std::string &file, bool doimages, bool full);
            void get_mimg(std::ostringstream &offset,std::string &file, bool doimages=true, bool full=false);
            bytes get_mimg(std::string file, bool doimages=true, bool full=false);

            void load_data(bytes data);
            void load_rgba32(bytes data);
            bytes get_rgba32();

            void read_any(std::string filename);

            void read_png(std::string filename);
            void write_png(std::string filename);
            void read_img(std::string filename);
            void write_img(std::string filename, bool doimages=true);
            void write_mimg(std::string filename, bool doimages=true, bool full=false);
            //void read_mimg(std::string filename);

            bytes decompress(bytes data, int type, int size);
            bytes compress(bytes data, int type, int size);

            void create_rgba32(image_data img);
            void add_alpha(image_data img=image_data());
            image_data split_rgba32();

            void align(int max_x=0, int max_y=0, int min_x=0, int min_y=0);
            void dealign();


        protected:

        private:
    };
}

#endif // IMAGE_H
