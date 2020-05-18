#include <iostream>
#include <string>
#include <vector>

#include "PNGformat.h"
#include "ANN.h"
#include "Image.h"
#include "filesys.h"

using namespace std;

enum mode{
    decode_ann,
    code_ann,
    decode_img,
    code_img
};

vector<string> filenames;

struct Both{
    bool file_dir=false;//sets output directory to input file's directory
    bool local=false;//creates directory for unpacked ann, same as input file's name
    string out_directory;//specifies output directory

    bool pad=false;
    int pad_number=0;

    bool log=true;

};
Both both;

//struct Code{
//
//};
//Code code;

struct Decode{
    bool sequence=false;//exports sequence
    string event_name;//name of event to extract
    bool sequenced=false;//extracts every sequence as another folder

    bool align=false;//resolves all position changes

    bool metafile=false;//creates mann file, ignores previous flags

};

Decode decode;

int get_mode(am::bytes data){
    string check((char*)data.data(),4);
    if(both.log)
        cout<<"Magic: "<<check<<endl;
    if(check.substr(0,3)=="NVP")
        return 0;
    else if(check.substr(0,3)=="ANN")
        return 1;
    else if(check.substr(0,3)=="PIK")
        return 2;
    else if(check=="\x89PNG")
        return 3;
    return -1;
}




int get_event_id(am::ANN &ann){
    if(decode.event_name.size()==0){
        cout<<endl;
        for(int ev=0;ev<ann.events.size();ev++){
            cout<<ev<<".\t"<<ann.events[ev].name<<endl;
        }
        cout<<"Choose event name: (type \":\") to get sequence by numer"<<endl;
        cin>>decode.event_name;
    }

    if(decode.event_name.compare(0,1,":")==0){
        return stoi(decode.event_name.substr(1));
    }else{
        for(int ev=0;ev<ann.events.size();ev++){
            if(decode.event_name.compare(ann.events[ev].name)==0){
                return ev;
            }
        }
    }

    throw invalid_argument(string("Can't find event with name: ")+decode.event_name);
}

void help_page(){
    cout<<"Decode >ANN<";
    cout<<endl<<endl;
    cout<<"anndrzem [file paths][-h][-f directory_path][-l][-o][-s event_name][-e][-n]"<<endl<<endl;
    cout<<"-h\tShows help message."<<endl;
    cout<<"-f\tSets output directory."<<endl;
    cout<<"-l\tSome random console output."<<endl;
    cout<<"-o\tDon't create new directory for images."<<endl;
    cout<<"-s\tCreates event sequence (type \":\" to get sequence by number)(events names are going to be listed and can be chosen from on runtime)."<<endl;
    cout<<"-n\tAlign image sizes."<<endl;
    cout<<endl;
}

string get_arg(char *command[],int &arg,int maxi);

void get_flag(char option,bool last,char *command[],int &arg,int maxi){
    switch(option){
    case 'h':
        help_page();
        break;
    case 'f':
        both.file_dir=true;
        break;
    case 'l':
        both.local=true;
        break;
    case 'o':
        both.pad=true;
        if(last)
            both.out_directory=get_arg(command,arg,maxi);//check for "-*"!!!!
        break;
    case 'p':
        if(last)
            both.pad_number=stoi(get_arg(command,arg,maxi));
        break;

    case 's':
        decode.sequence=true;
        if(last)
            decode.event_name=get_arg(command,arg,maxi);
        break;
    case 'q':
        decode.sequenced=true;//to implement
        break;
    case 'a':
        decode.align=true;
        break;

    case 'm':
        decode.metafile=true;
        break;
    }
}


string get_arg(char *command[],int &arg,int maxi){
    string str;
    if(arg<maxi){
        str=command[arg];
    }else{
        return "";
    }

    arg++;

    if(str[0]=='/'||str[0]=='-'){
        bool last=false;
        for(int ar=1;ar<str.size();ar++){
            if(ar=str.size()-1) last=true;
            get_flag(str[ar],last,command,arg,maxi);
        }
        return "";
    }

    return str;
}



void parse_commandline(char *command[],int maxi){
    string str;
    int arg=1;
    while(arg<maxi){
        str=get_arg(command,arg,maxi);
        if(str.size()>0){
            cout<<"File: "<<str<<endl;
            filenames.push_back(str);
        }
    }
}

void align_sequence(int event_id, am::ANN &ann){
    int min_pos_x=0;
    int min_pos_y=0;

    for(int fr=0;fr<ann.events[event_id].frames.size();fr++){
        ann.images[ann.events[event_id].frames[fr].image_ref].position_x+=ann.events[event_id].frames[fr].position_x;
        if(ann.images[ann.events[event_id].frames[fr].image_ref].position_x<min_pos_x)
            min_pos_x=ann.images[ann.events[event_id].frames[fr].image_ref].position_x;

        ann.images[ann.events[event_id].frames[fr].image_ref].position_y+=ann.events[event_id].frames[fr].position_y;
        if(ann.images[ann.events[event_id].frames[fr].image_ref].position_y<min_pos_y)
            min_pos_x=ann.images[ann.events[event_id].frames[fr].image_ref].position_y;
    }

    for(int fr=0;fr<ann.events[event_id].frames.size();fr++){
        ann.images[ann.events[event_id].frames[fr].image_ref].position_x+=min_pos_x;
        ann.images[ann.events[event_id].frames[fr].image_ref].position_y+=min_pos_y;
        ann.images[ann.events[event_id].frames[fr].image_ref].align();
    }
}




int main(int argc, char *argv[])
{
    if(argc>1){
        parse_commandline(argv,argc);

        for(string &filename : filenames){

            string out_dir;

            if(both.file_dir){
                out_dir=get_directory(filename);
            }else if(both.out_directory.size()>0){
                out_dir=both.out_directory;
            }else{
                out_dir="./";
            }

            if(!both.local){
                out_dir+=get_file_name(filename)+string("/");
                create_directory(out_dir);
            }

            if(both.log)
                cout<<out_dir<<endl;

            int what=get_mode(read_file(filename,4));
            if(both.log)
                cout<<"Mode: "<<what<<endl;

            if(what==decode_ann){
                am::ANN ann;
                ann.read_any(filename);

                if(decode.align&&!decode.sequence){
                    for(am::Image &image:ann.images){
                        image.align();
                    }
                }

                if(decode.metafile){
                    ann.write_mann(out_dir);
                }else if(decode.sequence){
                    int event_id=get_event_id(ann);

                    align_sequence(event_id,ann);


                    int pad_len;
                    if(both.pad_number>=len_int(ann.events[event_id].frames.size()))
                        pad_len=both.pad_number;
                    else
                        pad_len=len_int(ann.events[event_id].frames.size());

                    for(int fr=0;fr<ann.events[event_id].frames.size();fr++){
                        string number;
                        if(both.pad)
                            number=pad_int(fr,pad_len);
                        else
                            number=to_string(fr);

                        ann.images[ann.events[event_id].frames[fr].image_ref].write_png(ann.events[event_id].name+"_"+number+string(".png"));
                    }
                }else{
                    for(int im=0;im<ann.images.size();im++){
                        ann.images[im].write_png(out_dir+to_string(im)+string("_")+ann.images[im].name+string(".png"));
                    }
                }
            }else if(what==code_ann){
                am::ANN ann;
                ann.read_any(filename);
                ann.write_ann(out_dir+get_file_name(filename)+string(".ann"));
            }else if(what==decode_img){
                am::Image image;
                image.read_img(filename);
                if(decode.align){
                    image.align();
                }
                image.write_png(out_dir+get_file_name(filename)+string(".png"));
            }else if(what==code_img){
                am::Image image;
                image.read_png(filename);
                image.write_img(out_dir+get_file_name(filename)+string(".img"));
            }

        }
    }
    return 0;
}


