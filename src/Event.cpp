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
        frames.clear();
        dic dict;
        while(1){
            dict=get_val(offset);
            if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="loop"){
                loop_number=stoi(dict.value);
            }else if(dict.key=="frame"){
                log();
                while(dict.key=="frame"){
                    frames.push_back(Frame());
                    frames.back().image_ref=add_file(files,dict.value);
                    dict=frames.back().load_mann(offset,files);
                }
                return dict;
            }else{
                return dict;
            }
        }
        return dict;
    }

    void Event::get_ann(back_insert_iterator<bytes> &offset){
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
            frames[fr].get_ann(offset);
        }

    }

    void Event::get_mann(std::ostringstream &offset,std::vector<std::string>&files){
        offset<<"Event="<<name<<endl;
        if(loop_number!=0)
            offset<<"\tloop="<<loop_number<<endl;
        if(transparency!=255)
            offset<<"\ttransparency="<<transparency<<endl;
        for(int fr=0;fr<frames.size();fr++){
            frames[fr].get_mann(offset,files);
        }
    }


}
