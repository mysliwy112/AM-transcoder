#ifndef FRAME_H
#define FRAME_H

#include<string>
#include "am_utils.h"

namespace am{
    class Frame: public Graphics
    {
        public:
            Frame();

            void log();

            unsigned short image_ref;
            std::string check="`}U.";
            short position_x=0;
            short position_y=0;
            unsigned int sfx_switch=0;
            int transparency=255;
            std::string name="none";
            std::string sounds;

            void load_ann(bytes::iterator &offset);
            dic load_mann(std::stringstream &offset,std::vector<std::string>&files);

            void get_ann(std::back_insert_iterator<bytes> &offset);
            void get_mann(std::ostringstream &offset,std::vector<std::string>&files);

        protected:

        private:
    };
}
#endif // FRAME_H
