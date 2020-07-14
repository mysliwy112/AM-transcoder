#include "Frame.h"
#include <iostream>

using namespace std;

namespace am{
    Frame::Frame()
    {
        //ctor
    }

    void Frame::load_ann(bytes::iterator &offset){
        check=get_str(offset,0x4);
        advance(offset,0x4);

        position_x=get_int(offset,0x2);
        position_y=get_int(offset,0x2);

        advance(offset,0x4);

        sfx_switch=get_int(offset,0x4);

        advance(offset,0x4);

        transparency=get_int(offset,0x1);

        advance(offset,0x5);

        int name_size=get_int(offset,0x4);
        name=get_str(offset,name_size);

        if(sfx_switch!=0){
            int sfx_size=get_int(offset,0x4);
            sounds=get_str(offset,sfx_size);
        }
        log();
    }

    void Frame::log(){
        if(LOG){
            cout<<"Frame: "<<name<<endl;
            cout<<"Check: "<<check<<endl;
            cout<<"x:"<<position_x<<" y:"<<position_y<<endl;
            cout<<"sfx: "<<sounds<<endl;
            cout<<"Transparency: "<<transparency<<endl;
            cout<<endl;
        }
    }


    dic Frame::load_mann(stringstream &offset,vector<string>&files){
        dic dict;
        while(1){
            dict=get_val(offset);
            if(dict.key=="position_x"){
                position_x=stoi(dict.value);
            }else if(dict.key=="position_y"){
                position_y=stoi(dict.value);
            }else if(dict.key=="sfx_hash"||dict.key=="sfx_seed"){
                sfx_switch=stoi(dict.value);
            }else if(dict.key=="sfx"){
                sounds=dict.value;
                if(sfx_switch==0){
                    srand(sounds.size());
                    sfx_switch=rand();
                }
            }else if(dict.key=="name"){
                name=dict.value;
            }else if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="check"){
                check=dict.value;
            }else{
                log();
                return dict;
            }
        }

        return dict;
    }

    void Frame::get_ann(back_insert_iterator<bytes> &offset, bool doimages){
        set_str(offset,check,0x4);
        set_data(offset,bytes(0x4,0));
        set_int(offset,position_x,0x2);
        set_int(offset,position_y,0x2);
        set_data(offset,bytes(0x4,0xFF));
        set_int(offset,sfx_switch,0x4);
        set_data(offset,bytes(0x4,0));
        set_int(offset,transparency,0x1);
        set_data(offset,bytes(0x5,0));

        set_int(offset,name.size()+1,0x4);
        set_str(offset,name,name.size()+1);
        if(sfx_switch>0){
            set_int(offset,sounds.size()+1);
            set_str(offset,sounds,sounds.size()+1);
        }
    }

    void Frame::get_mann(std::ostringstream &offset,std::vector<std::string>& files, bool doimages, bool full){
        offset<<"\tFrame="<<files[image_ref]<<endl;
        if(full)
            offset<<"\t\tname="<<name<<endl;

        if(position_x!=0||full)
            offset<<"\t\tposition_x="<<position_x<<endl;
        if(position_y!=0||full)
            offset<<"\t\tposition_y="<<position_y<<endl;

        if(sfx_switch!=0){
            if(full)
                offset<<"\t\tsfx_seed="<<sfx_switch<<endl;
            offset<<"\t\tsfx="<<sounds<<endl;
        }
        if(full)
            offset<<"\t\tcheck="<<check<<endl;

    }


}
