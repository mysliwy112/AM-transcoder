#ifndef ANN_H
#define ANN_H

#include<string>
#include<vector>
#include<sstream>

#include"am_utils.h"
#include"Event.h"
#include"Image.h"

namespace am{

    struct settings{
        bool check=true;
    };


    //object representing ANN file
    class ANN: public Graphics
    {
        public:
            ANN(std::string name="no_name");

            std::string name;
            std::string author;
            int bpp;
            int transparency;

            std::vector<Event> events;
            std::vector<Image> images;

            void load(bytes file, std::string name);

            void load_header_ann(bytes::iterator &offset);
            using Graphics::load_ann;
            void load_ann(bytes::iterator &offset);
            using Graphics::load_mann;
            dic load_mann(std::stringstream &offset);


            bytes get_mann();

            void read_any(std::string filename);
            void write_mann(std::string filename);
            void write_ann(std::string filename);


            void align_image(Image& img, int max_x, int max_y, int min_x, int min_y, int bpp=4);


            bytes original;

            check_bound(bytes::iterator offset,unsigned long long length);



        protected:

            std::string validate_filename(std::string filename);
            bool testpath(std::string filename);

            private:
    };
}
#endif // ANN_H
