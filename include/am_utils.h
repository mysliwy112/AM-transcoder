#ifndef AM_UTILS_H
#define AM_UTILS_H

#include<vector>
#include<string>
#include <sstream>
#include <iterator>

namespace am{
    extern bool LOG;
    extern int PAD;
    typedef std::vector<unsigned char> bytes;

    struct dic{
        std::string key;
        std::string value;
    };

    class Graphics{
        public:
            Graphics();

            //bool log=false;

            long long get_int(bytes::iterator &offset,int length);
            void set_int(std::back_insert_iterator<bytes> &offset,unsigned long long number,int length=4);

            std::string get_str(bytes::iterator &offset,int length);
            void set_str(std::back_insert_iterator<bytes> &offset, std::string str, int length=0);

            bytes get_data(bytes::iterator &offset,int length);
            void set_data(std::back_insert_iterator<bytes> &offset,bytes data);

            dic get_val(std::stringstream &offset);

            void check_bound(bytes::iterator offset,unsigned long long length);

            unsigned long long combine(bytes::iterator value,int siz);

            virtual void load_ann(bytes::iterator &offset);
            void load_ann(bytes data);
            virtual dic load_mann(std::stringstream &offset,std::vector<std::string>&files);
            void load_mann(bytes data);


            virtual void get_ann(std::back_insert_iterator<bytes> &offset,bool doimages=true);
            bytes get_ann(bool doimages=true);
            virtual void get_mann(std::ostringstream &offset,std::vector<std::string>&files, bool doimages=true ,bool full=false);
            bytes get_mann(bool doimages=true, bool full=false);



            int add_file(std::vector<std::string> &files, std::string file);




    };
}

#endif // AM_UTILS_H
