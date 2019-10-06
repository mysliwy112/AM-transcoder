#ifndef ANN_H
#define ANN_H

#include<string>
#include<vector>
#include<fstream>

using namespace std;

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
        string name;
        vector<unsigned char> image_data;
        vector<unsigned char> alpha_data;
        vector<unsigned char> decode_data;
    };

    struct frame{
        unsigned short image_ref;
        string check;
        //int nwm1;
        short position_x;
        short position_y;
        unsigned int sfx_switch;
        //int nwm2;
        uint8_t transparency;
        //string nwm3;
        string name;
        string sounds;
    };

    struct event{
        string name;
        unsigned int frames_number;
        //int nwm1;
        unsigned int loop_number;
        //int nwm2;
        unsigned int transparency;
        //int nwm3;
        vector<frame> frames;
    };

    struct header{
        string name;
        string check;
        unsigned short images_number;
        unsigned short bpp;
        unsigned short events_number;
        //string opis1;
        //int8_t opis2;
        //string opis3;
        int transparency;
        //vector<char> dump;
        unsigned int caption_size;
        string caption;
    };

class ANN
{
    public:
        ANN();

        header head;
        string name;
        vector<event> events;
        vector<image> images;

        bool is_seqence=false;
        string seqence_name;

        bool align=false;

        bool to_raw=false;

        bool log=false;

        bool is_bigfile=false;

        void set_align(bool a);
        void set_raw(bool a);
        void set_log(bool a);
        void set_sequence(bool is, string s={});

        void load_ann(string filename);
        void extract_ANN(string directory);

        int pad_int(int number);

        void align_image(image &img, int max_x, int max_y, int min_x, int min_y, int bpp=4);
        void decodeImage(image &img);

        void extract_to_png(image img, string filename);
        void extract_to_file(vector<unsigned char> code,string filename);

    protected:

        vector<unsigned char> read_file(string filename);
        vector<unsigned char> read_file(ifstream &file, long long f_size);

        unsigned long long combine(unsigned char* value,int number);
        vector<unsigned char> link(vector<unsigned char> color, vector<unsigned char> alpha);
        vector<unsigned char> to_24bpp(vector<unsigned char> color);

        int get_event_id(string name);

        private:
};

#endif // ANN_H
