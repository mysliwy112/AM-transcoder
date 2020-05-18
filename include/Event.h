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

            std::string name="STAND";
            unsigned int loop_number=0;
            unsigned int transparency=255;
            std::vector<Frame> frames;

            void load_ann(bytes::iterator &offset);
            dic load_mann(std::stringstream &offset,std::vector<std::string>&files);

            void get_ann(bytes::iterator &offset);
            void get_mann(bytes::iterator &offset);

        protected:

        private:
    };
}

#endif // EVENT_H
