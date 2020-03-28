#ifndef ANN_H
#define ANN_H

#include<string>
#include<vector>
#include<fstream>

struct image{
    unsigned short width;
    unsigned short height;
    short position_x;
    short position_y;
    unsigned short compression;
    unsigned int image_size;
    //int nwm1;
    //string nwm2;
    unsigned int alpha_size;
    std::string name;
    std::vector<unsigned char> image_data;
    std::vector<unsigned char> alpha_data;
    std::vector<unsigned char> decode_data;
};

struct frame{
    unsigned short image_ref;
    std::string check;
    //int nwm1;
    short position_x;
    short position_y;
    unsigned int sfx_switch;
    //int nwm2;
    uint8_t transparency;
    //string nwm3;
    std::string name;
    std::string sounds;
};

struct event{
    std::string name;
    unsigned int frames_number;
    //int nwm1;
    unsigned int loop_number;
    //int nwm2;
    unsigned int transparency;
    //int nwm3;
    std::vector<frame> frames;
};

struct header{
    std::string name;
    std::string check;
    unsigned short images_number;
    unsigned short bpp;
    unsigned short events_number;
    //string opis1;
    //int8_t opis2;
    //string opis3;
    int transparency;
    //vector<char> dump;
    unsigned int caption_size;
    std::string caption;
};


//object representing ANN file
class ANN
{
    public:
        ANN();

        header head;
        std::string name;
        std::vector<event> events;
        std::vector<image> images;

        bool is_seqence=false;
        std::string seqence_name;

        bool align=false;
        bool to_raw=false;
        bool log=false;
        bool is_bigfile=false;

        //setters
        void set_align(bool a);
        void set_raw(bool a);
        void set_log(bool a);
        void set_sequence(bool is, std::string s={});

        void load_ann(std::string filename);
        void extract_ANN(std::string directory);

        //returns number of digits in given number
        int pad_int(int number);

        void align_image(image &img, int max_x, int max_y, int min_x, int min_y, int bpp=4);
        void decodeImage(image &img);

        //saves image as png images
        void extract_to_png(image img, std::string filename);
        //saves image as ran raw bytes
        void extract_to_file(std::vector<unsigned char> code,std::string filename);

    protected:

        std::vector<unsigned char> read_file(std::string filename);
        std::vector<unsigned char> read_file(std::ifstream &file, long long f_size);

        std::string validate_filename(std::string filename);
        bool testpath(std::string filename);

        //links RGB channels and alpha channel
        std::vector<unsigned char> link(std::vector<unsigned char> color, std::vector<unsigned char> alpha);
        //converts 16bpp RGB to 24bpp
        std::vector<unsigned char> to_24bpp(std::vector<unsigned char> color);

        //combines bytes into bigger integer
        unsigned long long combine(unsigned char* value,int siz);

        int get_event_id(std::string name);

        private:
};

#endif // ANN_H
