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
        if(log){
            cout<<"Frame: "<<name<<endl;
            cout<<"Check: "<<check<<endl;
            cout<<"x:"<<position_x<<" y:"<<position_y<<endl;
            cout<<"sfx: "<<sounds<<endl;
            cout<<"Transparency: "<<transparency<<endl;
            cout<<endl;
        }
    }

    dic Frame::load_mann(stringstream &offset,vector<string>&files){
        dic dict=get_val(offset);
        while(1){
            if(dict.key=="position_x"){
                position_x=stoi(dict.value);
            }else if(dict.key=="position_y"){
                position_y=stoi(dict.value);
            }else if(dict.key=="sfx"){
                sounds=dict.value;
            }else if(dict.key=="name"){
                name=dict.value;
            }else if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="frame"){
                return dict;
            }else{
                cout<<"Invalid key: "<<dict.key<<endl;
            }
            dic dict=get_val(offset);
        }
        return dict;
    }
}
