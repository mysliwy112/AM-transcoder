#ifndef EVENT_H
#define EVENT_H

#include<vector>

#include"am_utils.h"
#include"Frame.h"


namespace am{
    class Event : public Graphics
    {
        public:
            Event();

            std::string name;
            unsigned int loop_number;
            unsigned int transparency;
            std::vector<Frame> frames;

            void load_ann(bytes::iterator &offset);
            void load_mann(bytes::iterator &offset);

            void get_ann(bytes::iterator &offset);
            void get_mann(bytes::iterator &offset);

        protected:

        private:
    };
}

#endif // EVENT_H
