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

            void log();

            std::string name="STAND";
            unsigned int loop_number=0;
            unsigned int transparency=255;
            std::vector<Frame> frames;

            void load_ann(bytes::iterator &offset);
            dic load_mann(std::stringstream &offset,std::vector<std::string>&files);
            void load_jann(nlohmann::json &fj,std::vector<std::string>&files);

            void get_ann(std::back_insert_iterator<bytes> &offset, bool doimages);
            void get_mann(std::ostringstream &offset,std::vector<std::string>&files, bool doimages, bool full);
            nlohmann::json get_jann(std::vector<std::string>&files, bool doimages, bool full);

        protected:

        private:
    };
}

#endif // EVENT_H
