#ifndef FRAME_H
#define FRAME_H

#include<string>
#include "am_utils.h"

namespace am{
    class Frame: public Graphics
    {
        public:
            Frame();

            unsigned short image_ref;
            std::string check;
            short position_x;
            short position_y;
            unsigned int sfx_switch;
            int transparency;
            std::string name;
            std::string sounds;

            void load_ann(bytes::iterator &offset);
            void load_mann(bytes::iterator &offset);

            void get_ann(bytes::iterator &offset);
            void get_mann(bytes::iterator &offset);

        protected:

        private:
    };
}
#endif // FRAME_H
