#include "ANN.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdexcept>
#include<algorithm>

#include "filesys.h"

using namespace std;

namespace am{
    ANN::ANN(string ann_name){
        name=ann_name;
    }


    void ANN::load_header_ann(bytes::iterator &offset){
        string check=get_str(offset,0x4);
        if(LOG){
            if(check.compare("NVP")!=0){
                cout<<"Warning: Inappropriate check string: "<<check<<endl;
            }else{
                cout<<"It's ann file!"<<endl;
            }
        }

        images.resize(get_int(offset,0x2));
        bpp=get_int(offset,0x2);
        events.resize(get_int(offset,0x2));

        //unknown data
        advance(offset,0xD);//kuratoren
        advance(offset,0x4);
        advance(offset,0x4);

        transparency=get_int(offset,0x1);

        advance(offset,0xC);

        unsigned int caption_size=get_int(offset,0x4);
        author=get_str(offset,caption_size);

        advance(offset,0x4);
        log();

    }

    void ANN::log(){
        if(LOG){
            cout<<"Loaded header:"<<endl;
            cout<<"Number of images: "<<images.size()<<endl;
            cout<<"Number of events: "<<events.size()<<endl;
            cout<<"Author: "<<author<<endl;
            cout<<"Bpp: "<<bpp<<endl;
            cout<<"Transparency: "<<author<<endl;
        }
    }

    void ANN::load_ann(bytes::iterator &offset){

        if(LOG)
            cout<<"Loading header"<<endl;

        load_header_ann(offset);
        if(LOG)
            cout<<"Loading events & frames"<<endl;

        for(int ev=0;ev<events.size();ev++){
            events[ev].load_ann(offset);
        }

        if(LOG)
            cout<<"Loading images"<<endl;
        for(int im=0;im<images.size();im++){
            images[im].load_ann(offset);
            images[im].bpp=bpp;
        }

        for(int im=0;im<images.size();im++){
            if(LOG)
                cout<<"Loading data...";
            images[im].load_data(get_data(offset,images[im].image_size+images[im].alpha_size));
            if(LOG)
                cout<<"completed"<<endl;
        }
        if(LOG)
            cout<<"\n\n\nAnn file has been loaded\n\n\n";
    }

    dic ANN::load_mann(stringstream &offset, vector<string>&files){

        dic dict;
        string check;
        offset>>check;
        if(LOG){
            if(check.compare("ANN")!=0){
                cout<<"Warning: Inappropriate check string"<<endl;
            }else{
                cout<<"It's meta ann file!"<<endl;
            }
        }
        events.clear();
        images.clear();


        while(1){
            dict=get_val(offset);
            if(dict.key=="transparency"){
                transparency=stoi(dict.value);
            }else if(dict.key=="author"){
                author=dict.value;
            }else if(dict.key=="bpp"){
                bpp=stoi(dict.value);
            }else if(dict.key=="name"){
                name=dict.value;
            }else if(dict.key=="event"){
                log();
                while(dict.key=="event"){
                    events.push_back(Event());
                    events.back().name=dict.value;
                    dict=events.back().load_mann(offset,files);
                }
                images.resize(files.size());
                for(int im=0;im<images.size();im++){
                    images[im].name=get_file_name(files[im]);
                    images[im].read_png(mann_dir+files[im]);
                }
                return dict;
            }else{
                cout<<"Invalid key: "<<dict.key<<endl;
                throw"Invalid key-header";
            }
        }
        return dict;
    }


    void ANN::load(bytes file, string name){
        original=file;
        name=name;

        int mode=0;

        string check((char*)file.data(),3);
        if (check=="NVP"){
            mode=1;
        }else if(check=="ANN"){
            mode=2;
        }

        if(mode==0){
            throw invalid_argument("Unknown filetype");
        }else if(mode==1){
            load_ann(file);
        }else if(mode==2){
            load_mann(file);
        }
    }

    void ANN::get_ann(back_insert_iterator<bytes> &offset){
        set_str(offset,"NVP",0x4);
        set_int(offset,images.size(),0x2);
        set_int(offset,bpp,0x2);
        set_int(offset,events.size(),0x2);

        set_str(offset,":kuratoren:",0xD);
        set_int(offset,23,0x4);
        set_data(offset,bytes(4,0));
        set_int(offset,transparency,0x1);
        set_data(offset,bytes(0xC,0));
        set_int(offset,author.size(),0x4);
        set_str(offset,author);
        set_data(offset,bytes(0x4,0));
        for(int ev=0;ev<events.size();ev++){
            events[ev].get_ann(offset);
        }

        //good for now
        vector<image_data> im_datas(images.size());
        for(int im=0;im<images.size();im++){
            im_datas[im]=images[im].get_ann();
            set_data(offset,im_datas[im].header);
        }
        for(int im=0;im<images.size();im++){
            set_data(offset,im_datas[im].image);
            set_data(offset,im_datas[im].alpha);
        }

    }

    void ANN::get_mann(ostringstream &offset,vector<std::string>&files){
        offset<<"ANN\n\n";
        offset<<"name="<<name<<endl;
        offset<<"author="<<author<<endl;
        if(transparency!=255)
            offset<<"transparency="<<transparency<<endl;
        if(bpp!=16)
            offset<<"bpp="<<bpp<<endl;
        files.resize(images.size());
        for(int im=0;im<images.size();im++){
            files[im]=to_string(im)+"_"+images[im].name+".png";
            images[im].align();
            images[im].write_png(mann_dir+files[im]);
        }
        for(int ev=0;ev<events.size();ev++){
            events[ev].get_mann(offset,files);
        }
    }




    void ANN::read_any(string filename){
        bytes data=read_file(filename);
        mann_dir=get_directory(filename);
        load(data,get_file_name(filename));
    }


    void ANN::write_mann(string filename){
        mann_dir=filename;
        bytes data=get_mann();
        write_file(filename+name+".mann",data);
    }

    void ANN::write_ann(string filename){
        bytes data=get_ann();
        write_file(filename,data);
    }
};
