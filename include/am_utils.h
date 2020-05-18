#ifndef AM_UTILS_H
#define AM_UTILS_H

#include<vector>
#include<string>
#include <sstream>

namespace am{
    typedef std::vector<unsigned char> bytes;

    struct dic{
        std::string key;
        std::string value;
    };

    class Graphics{
        public:
            Graphics();

            bool log=true;

            unsigned long long get_int(bytes::iterator &offset,int length);
            void set_int(bytes::iterator &offset,unsigned long long number,int length=4);

            std::string get_str(bytes::iterator &offset,int length);
            void set_str(bytes::iterator &offset, std::string str, int length=0);

            bytes get_data(bytes::iterator &offset,int length);
            void set_data(bytes::iterator &offset,bytes data);

            dic get_val(std::stringstream &offset);

            void check_bound(bytes::iterator offset,unsigned long long length);

            unsigned long long combine(bytes::iterator value,int siz);

            virtual void load_ann(bytes::iterator &offset);
            void load_ann(bytes data);
            virtual dic load_mann(std::stringstream &offset);
            void load_mann(bytes data);

            bytes get_ann();
            bytes get_mann();



    };
}

#endif // AM_UTILS_H
