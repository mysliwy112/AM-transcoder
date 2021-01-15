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
            ANN(std::string name="");

            void log();

            //name of output file
            std::string name="";
            std::string author="anon";

            int bpp=16;
            int transparency=255;

            std::vector<Event> events;
            std::vector<Image> images;

            void load(bytes file);

            void load_header_ann(bytes::iterator &offset);

            using Graphics::load_ann;
            void load_ann(bytes::iterator &offset);
            using Graphics::load_mann;
            dic load_mann(std::stringstream &offset,std::vector<std::string>&files);
            using Graphics::load_jann;
            void load_jann(nlohmann::json &fj,std::vector<std::string>&files);

            using Graphics::get_ann;
            void get_ann(std::back_insert_iterator<bytes> &offset ,bool doimages=true);
            using Graphics::get_mann;
            void get_mann(std::ostringstream &offset,std::vector<std::string>&files ,bool doimages=true , bool full=false);
            using Graphics::get_jann;
            nlohmann::json get_jann(std::vector<std::string>&files ,bool doimages=true , bool full=false);

            void read_any(std::string filename);
            void write_mann(std::string filename, bool doimages=true , bool full=false);
            void write_jann(std::string filename, bool doimages=true , bool full=false);
            void write_ann(std::string filename, bool doimages=true);

            int get_event_index(std::string ev_name);

            std::string mann_dir="";

            void align_images();
            void align_sequence(std::vector<Image> &img, int event_id);
            std::vector<Image> align_sequences();
            void align(std::vector<Image> &img);

            bytes original;

            check_bound(bytes::iterator offset,unsigned long long length);

        protected:
            //these don't work
            std::string validate_filename(std::string filename);
            bool testpath(std::string filename);

        private:
    };
}
#endif // ANN_H
