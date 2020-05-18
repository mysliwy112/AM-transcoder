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
        if(log){
            cout<<"Event: "<<name<<endl;
            cout<<"Number of frames: "<<frames.size()<<endl;
            cout<<"Loop: "<<loop_number<<endl;
            cout<<"Transparency: "<<transparency<<endl;
        }

        for(unsigned int fr=0;fr<frames.size();fr++){
            frames[fr].image_ref=get_int(offset,0x2);
            if(log)
                cout<<"Frame number "<<fr<<" got image id: "<<frames[fr].image_ref<<endl;
        }

        for(unsigned int fr=0;fr<frames.size();fr++){
            frames[fr].load_ann(offset);
        }
        if(log)
            cout<<"Event "<<name<<" completed"<<"\n\n\n";
    }

    dic Event::load_mann(stringstream &offset,vector<string>&files){
        frames.clear();
        dic dict=get_val(offset);
        while(1){
            if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="loop"){
                loop_number=stoi(dict.value);
            }else if(dict.key=="frame"){
                while(dict.key=="frame"){
                    frames.push_back(Frame());
                    frames.back().image_ref=add_file(files,dict.value);
                    dict=frames.back().load_mann(offset,files);
                }
                return dict;
            }else{
                cout<<"Invalid key: "<<dict.key<<endl;
            }
            dic dict=get_val(offset);
        }
        return dict;
    }
}
