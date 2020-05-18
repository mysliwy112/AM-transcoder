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
        if(log){
            string check=get_str(offset,0x4);
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
        if(log){
            cout<<"Loaded header:"<<endl;
            cout<<"Number of images: "<<images.size()<<endl;
            cout<<"Number of events: "<<events.size()<<endl;
            cout<<"Author: "<<author<<endl;
            cout<<"Bpp: "<<bpp<<endl;
            cout<<"Transparency: "<<author<<endl;
        }
    }

    void ANN::load_ann(bytes::iterator &offset){

        if(log)
            cout<<"Loading header"<<endl;

        load_header_ann(offset);
        if(log)
            cout<<"Loading events & frames"<<endl;

        for(int ev=0;ev<events.size();ev++){
            events[ev].load_ann(offset);
        }

        if(log)
            cout<<"Loading images"<<endl;
        for(int im=0;im<images.size();im++){
            images[im].load_ann(offset);
            images[im].bpp=bpp;
        }

        for(int im=0;im<images.size();im++){
            if(log)
                cout<<"Loading data"<<endl;
            images[im].load_data(get_data(offset,images[im].image_size+images[im].alpha_size));
            if(log)
                cout<<"Data loaded"<<endl;
        }
        if(log)
            cout<<"Ann file has been loaded"<<"\n\n\n";
    }

    dic ANN::load_mann(stringstream &offset){
        dic dict;
        string check;
        offset>>check;
        if(log){
            if(check.compare("ANN")!=0){
                cout<<"Warning: Inappropriate check string"<<endl;
            }else{
                cout<<"It's meta ann file!"<<endl;
            }
        }
        events.clear();
        images.clear();

        dict=get_val(offset);
        if(dict.key=="transparency"){
            transparency=stoi(dict.value);
        }else if(dict.key=="author"){
            author=dict.value;
        }else if(dict.key=="bpp"){
            author=dict.value;
        }else if(dict.key=="event"){
            author=dict.value;
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


    void ANN::read_any(string filename){
        bytes data=read_file(filename);
        cout<<data.size()<<endl;
        load(data,get_file_name(filename));
    }


    void ANN::write_mann(string filename){

    }

    void ANN::write_ann(string filename){

    }

    string ANN::validate_filename(string filename){
        int i=0;
        while(i<filename.size()){
            i=filename.find_first_of("<>:\"/\\|?*",i);
            if(i==string::npos){
                break;
            }
            filename.replace(i,1,"+");
            i++;
        }
        return filename;
    }

    bool ANN::testpath(string filename){
        ofstream test(filename);
        if(!test.good()){
            cout<<"Can't save file: "<<filename<<endl;
            test.close();
            return 0;
        }
        if(log){
            cout<<"Saving to: "<<filename<<endl;
        }
        test.close();
        return 1;
    }

};
