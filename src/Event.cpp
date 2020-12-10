#include "Event.h"
#include <iostream>

using namespace std;

namespace am{
    Event::Event()
    {
        //ctor
    }

    void Event::load_ann(bytes::iterator &offset){

        name=get_str(offset,0x20);
        unsigned int r=get_int(offset,0x2);
        frames.resize(r);

        advance(offset,0x6);

        loop_number=get_int(offset,0x4);

        advance(offset,0x4);
        advance(offset,0x6);

        transparency=get_int(offset,0x1);

        advance(offset, 0xC);

        log();

        for(unsigned int fr=0;fr<frames.size();fr++){
            frames[fr].image_ref=get_int(offset,0x2);
            if(LOG)
                cout<<"Frame number "<<fr<<" got image id: "<<frames[fr].image_ref<<endl;
        }

        for(unsigned int fr=0;fr<frames.size();fr++){
            frames[fr].load_ann(offset);
        }
        if(LOG)
            cout<<"Event "<<name<<" completed"<<"\n\n\n";
    }

    void Event::log(){
        if(LOG){
            cout<<"Event: "<<name<<endl;
            cout<<"Number of frames: "<<frames.size()<<endl;
            cout<<"Loop: "<<loop_number<<endl;
            cout<<"Transparency: "<<transparency<<endl;
        }
    }

    dic Event::load_mann(stringstream &offset,vector<string>&files){
        //frames.clear();
        dic dict;
        while(1){
            dict=get_val(offset);
            if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="loop"){
                loop_number=stoi(dict.value);
            }else if(dict.key=="frame"){
                log();
                int licz=0;
                while(dict.key=="frame"){
                    if(licz>=frames.size())
                        frames.push_back(Frame());
                    frames[licz].image_ref=add_file(files,dict.value);
                    dict=frames[licz].load_mann(offset,files);
                    licz++;
                }
                return dict;
            }else{
                return dict;
            }
        }
        return dict;
    }

    void Event::load_jann(nlohmann::json &fj,vector<string>&files){
        //frames.clear();

        try{
            transparency=fj.at("transparency");
        }catch(...){}
        try{
            loop_number=fj.at("loop");
        }catch(...){}
        log();
        int licz=0;
        for(auto& elem : fj["frames"].items()){
            if(licz>=frames.size())
                frames.push_back(Frame());
            frames[licz].load_jann(elem.value(),files);
            licz++;
        }
    }

    void Event::get_ann(back_insert_iterator<bytes> &offset, bool doimages){
        set_str(offset,name,0x20);
        set_int(offset,frames.size(),0x2);
        set_data(offset,bytes(0x6,0));

        set_int(offset,loop_number,0x4);
        set_int(offset,5,0x4);
        set_data(offset,bytes(0x6,0));

        set_int(offset,transparency,0x1);
        set_data(offset,bytes(0xC,0));
        for(int fr=0;fr<frames.size();fr++){
            set_int(offset,frames[fr].image_ref,0x2);
        }

        for(int fr=0;fr<frames.size();fr++){
            frames[fr].get_ann(offset, doimages);
        }

    }

    void Event::get_mann(std::ostringstream &offset,std::vector<std::string>&files, bool doimages, bool full){
        offset<<"Event="<<name<<endl;
        if(loop_number!=0||full)
            offset<<"\tloop="<<loop_number<<endl;
        if(transparency!=255||full)
            offset<<"\ttransparency="<<transparency<<endl;
        offset<<endl;
        for(int fr=0;fr<frames.size();fr++){
            frames[fr].get_mann(offset,files, doimages, full);
            offset<<endl;
        }
    }

    nlohmann::json Event::get_jann(std::vector<std::string>&files, bool doimages, bool full){
        nlohmann::json fj;
        if(loop_number!=0||full)
            fj["loop"]=loop_number;
        if(transparency!=255||full)
             fj["transparency"]=transparency;
        fj["frames"]=nlohmann::json::array();
        if(LOG){
            cout<<fj<<endl;
        }
        for(int fr=0;fr<frames.size();fr++){
            fj["frames"].push_back(frames[fr].get_jann(files, doimages, full));
        }
        return fj;
    }


}
