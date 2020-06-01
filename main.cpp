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

    bool pad=false;//add leading zeros to output filename
    int pad_number=0;

    bool log=false;

    bool align=false;//resolves all position changes

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


    int offset=0;//set offset for additional editing

    bool metafile=false;//creates mann file, ignores previous flags

};

Decode decode;

int get_mode(am::bytes data){
    string check((char*)data.data(),4);
    if(both.log)
        cout<<"Magic: "<<check<<endl;
    if(check.substr(0,3)=="NVP"||check.substr(0,3)=="NVM")
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
    string event_name=decode.event_name;
    if(event_name.size()==0){
        cout<<endl;
        for(int ev=0;ev<ann.events.size();ev++){
            cout<<ev<<".\t"<<ann.events[ev].name<<endl;
        }
        cout<<"Choose event name: (type \":\") to get sequence by numer"<<endl;
        cin>>event_name;
    }

    if(event_name.compare(0,1,":")==0){
        return stoi(event_name.substr(1));
    }else{
        for(int ev=0;ev<ann.events.size();ev++){
            if(event_name.compare(ann.events[ev].name)==0){
                return ev;
            }
        }
    }

    throw invalid_argument(string("Can't find event with name |")+event_name+"|");
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
    case 'v':
        both.log=true;
        am::LOG=true;
        break;
    case 'd':
        if(last)
            both.out_directory=get_arg(command,arg,maxi);//check for "-*"!!!!
        break;
    case 'p':
        both.pad=true;
        if(last){
            string str=get_arg(command,arg,maxi);
            if(str.size()>0)
                both.pad_number=stoi(str);
        }
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
        both.align=true;
        break;
    case 'o':
        if(last)
            decode.offset=stoi(get_arg(command,arg,maxi));
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
            if(ar==str.size()-1) last=true;
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
            if(both.log)
                cout<<"File: "<<str<<endl;
            filenames.push_back(str);
        }
    }
}


int get_pad_len(int size){
    int pad_len;
    if(both.pad_number>=len_int(size))
        pad_len=both.pad_number;
    else
        pad_len=len_int(size);
    return pad_len;
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
                out_dir=get_directory(argv[0]);
            }

            if(!both.local){
                out_dir+=get_file_name(filename)+string("/");

            }

            create_directory(out_dir);


            if(both.log)
                cout<<"Out directory: "<<out_dir<<endl;

            int what=get_mode(read_file(filename,4));
            if(both.log)
                cout<<"Mode: "<<what<<endl;

            if(what==decode_ann){
                am::ANN ann(get_file_name(filename));
                ann.read_any(filename);

                if(both.align&&!decode.sequence){
                    if(both.log)
                        cout<<"File align"<<endl;
                    ann.align_images();
                    if(both.log)
                        cout<<"completed"<<endl;
                }

                if(decode.offset&&!decode.sequence){
                    for(am::Image&image:ann.images)
                        image.align(image.position_x+image.width+decode.offset,
                                    image.position_y+image.height+decode.offset,
                                    image.position_x-decode.offset,
                                    image.position_y-decode.offset);
                }

                if(decode.metafile){
                    ann.write_mann(out_dir);
                }else if(decode.sequence){

                    int event_id=get_event_id(ann);

                    if(both.align){
                        ann.align_sequence(event_id);
                    }

                    if(decode.offset){
                        for(am::Image&image:ann.images)
                            image.align(image.position_x+image.width+decode.offset,
                                        image.position_y+image.height+decode.offset,
                                        image.position_x-decode.offset,
                                        image.position_y-decode.offset);
                    }

                    int pad_len=get_pad_len(ann.events[event_id].frames.size());

                    for(int fr=0;fr<ann.events[event_id].frames.size();fr++){

                        string number;
                        if(both.pad)
                            number=pad_int(fr,pad_len);
                        else
                            number=to_string(fr);

                        ann.images[ann.events[event_id].frames[fr].image_ref].write_png(out_dir+ann.events[event_id].name+"_"+number+".png");
                    }
                }else{

                    int pad_len=get_pad_len(ann.images.size());

                    for(int im=0;im<ann.images.size();im++){

                        string number;
                        if(both.pad)
                            number=pad_int(im,pad_len);
                        else
                            number=to_string(im);

                        ann.images[im].write_png(out_dir+ann.name+"_"+number+".png");
                    }
                }
            }else if(what==code_ann){
                am::ANN ann(get_file_name(filename));
                ann.read_any(filename);
                if(both.align){
                    for(am::Image& image : ann.images){
                        image.dealign();
                    }
                }
                ann.write_ann(out_dir+get_file_name(filename)+".ann");
            }else if(what==decode_img){
                am::Image image;
                image.read_img(filename);
                if(both.align){
                    image.align();
                }
                if(decode.offset){
                    image.align(image.position_x+image.width+decode.offset,
                                image.position_y+image.height+decode.offset,
                                image.position_x-decode.offset,
                                image.position_y-decode.offset);
                }
                image.write_png(out_dir+get_file_name(filename)+".png");
            }else if(what==code_img){
                am::Image image;
                image.read_png(filename);
                if(both.align){
                    image.dealign();
                }
                image.write_img(out_dir+get_file_name(filename)+".img");
            }

        }
    }
    return 0;
}


