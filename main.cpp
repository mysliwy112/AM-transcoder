#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "PNGformat.h"
#include "ANN.h"
#include "Image.h"
#include "filesys.h"
#include "am_utils.h"

using namespace std;

enum mode{
    decode_ann,
    code_ann,
    decode_img,
    code_img
};

vector<string> filenames;


struct Flag_main{
    virtual void set_arg(string arg){}
    virtual string get_arg(){return "nic";}
    string name;
    bool v;
    string desc;
};
template <typename arg_type = bool>
struct Flag : public Flag_main{
    Flag(vector<Flag_main*> &flag_list, string name, string desc, bool v=false, arg_type arg=0){
        flag_list.push_back((Flag_main*)this);
        this->name=name;
        this->v=v;
        this->arg=arg;
        this->desc=desc;
    }
    void set_arg(string arg){
        if constexpr(is_arithmetic<arg_type>::value){
            this->arg=stoi(arg);
        }else{
            this->arg=arg;
        }
    }
    string get_arg(){
        std::stringstream ss;
        ss<<arg;
        return ss.str();
    }
    arg_type arg;
    operator bool() const { return v; }
};


vector<Flag_main*> flag_list;
namespace flags{
    Flag<> help(flag_list,"help","Shows help message.",false);
    Flag<> file_dir(flag_list,"file","Sets output to input file's director.",false);
    Flag<> name_dir(flag_list,"nameOff","Stops creation of directory named by input ann's name.",true);
    Flag<string> out_directory(flag_list,"directory","Sets output directory.",false,"./");
    Flag<int> pad(flag_list,"pad","Pads numbers with zeros to specified number of digits.",false,0);
    Flag<> log(flag_list,"verbose","Generates additional console output, used mainly for debbuging.",false);
    Flag<> align(flag_list,"align","Align image sizes.",false);
    Flag<> ignore(flag_list,"ignore","Ignores errors.",false);
    Flag<string> sequence(flag_list,"sequence","Creates event sequence (type \":\" to get sequence by number)(events names are going to be listed and can be chosen from on runtime).",false,"");
    //Flag<> sequenced(flag_list,"sequence","Extracts every sequence as another folder.",false);
    Flag<int> offset(flag_list,"offset","Adds transparent pixels to all sides of image.",false,10);
    Flag<> metafile(flag_list,"metafile","Creates Meta file and additional images, used for encoding anns and imgs.",false);
};


void help_page(){
    cout<<"Transcode ANN and IMG";
    cout<<endl<<endl;
    int namelong=0;
    for(Flag_main* &flag:flag_list){
        if(flag->name.size()>namelong)
            namelong=flag->name.size();
    }
    for(Flag_main* &flag:flag_list){
        cout<<"-"<<flag->name[0]<<" "<<flag->name<<setw(namelong+1+flag->desc.size()-flag->name.size())<<flag->desc<<endl;
    }
}

bool get_flags(string command){
    if(command[1]!='-'){
        int part=1;
        while(command[part]!='='&&part<command.size()){
            int i=0;
            for(Flag_main* &flag:flag_list){
                if(flag->name[0]==command[part]){
                    flag->v=!flag->v;
                    if(part+1<command.size()&&command[part+1]=='='){
                        flag->set_arg(command.substr(part+2));
                        part=command.size();
                    }
                    break;
                }
                i++;
            }

            if(i==flag_list.size()){
                return false;
                cout<<"Unknown flag "<<command[part]<<endl;
                throw "Unknown flag";
            }
            part++;
        }
    }else if(command[1]=='-'){
        for(Flag_main* &flag:flag_list){
            if(command.compare(2,command.find_first_of('=')-2,flag->name)==0){
                flag->v=!flag->v;
                if(command.find_first_of('=')!=string::npos){
                    flag->set_arg(command.substr(command.find_first_of('=')+1));
                }
                break;
            }
        }
    }
    return true;
}

void parse_commandline(char *commands[],int maxi){
    string str;
    int arg=1;
    while(arg<maxi){
        str=commands[arg];
        bool flag=false;
        if(str[0]=='-')
            flag=get_flags(str);
        if(flag==false)
            filenames.push_back(str);
        arg++;
    }
}





int get_mode(am::bytes data){
    string check((char*)data.data(),4);
    if(flags::log)
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
    string sequence=flags::sequence.arg;
    if(sequence.size()==0){
        cout<<endl;
        for(int ev=0;ev<ann.events.size();ev++){
            cout<<ev<<".\t"<<ann.events[ev].name<<endl;
        }
        cout<<"Choose event name: (type \":\") to get sequence by numer"<<endl;
        cin>>sequence;
    }

    if(sequence.compare(0,1,":")==0){
        return stoi(sequence.substr(1));
    }else{
        for(int ev=0;ev<ann.events.size();ev++){
            if(sequence.compare(ann.events[ev].name)==0){
                return ev;
            }
        }
    }

    throw invalid_argument(string("Can't find event with name |")+sequence+"|");
}

int main(int argc, char *argv[])
{
    if(argc>1){
        parse_commandline(argv,argc);
        if(flags::help)
            help_page();
        if(flags::pad)
            am::PAD=flags::pad.arg;
        am::LOG=flags::log;

        for(string &filename : filenames){
            try{
                string out_dir;

                if(flags::file_dir){
                    out_dir=get_directory(filename);
                }else if(flags::out_directory.arg.size()>0){
                    out_dir=flags::out_directory.arg;
                }else{
                    out_dir=get_directory(argv[0]);
                }

                int what=get_mode(read_file(filename,4));

                if(flags::log)
                    cout<<"Mode: "<<what<<endl;

                if(flags::name_dir&&what==decode_ann){
                    out_dir+=get_file_name(filename)+string("/");
                }

                create_directory(out_dir);

                if(flags::log)
                    cout<<"Out directory: "<<out_dir<<endl;


                if(what==decode_ann){
                    am::ANN ann(get_file_name(filename));
                    ann.read_any(filename);


                    if(flags::align&&!flags::sequence){
                        if(flags::log)
                            cout<<"File align"<<endl;
                        ann.align_images();
                        if(flags::log)
                            cout<<"completed"<<endl;
                    }

                    if(flags::offset.arg&&!flags::sequence){
                        for(am::Image&image:ann.images)
                            image.align(image.position_x+image.width+flags::offset.arg,
                                        image.position_y+image.height+flags::offset.arg,
                                        image.position_x-flags::offset.arg,
                                        image.position_y-flags::offset.arg);
                    }

                    if(flags::metafile){
                        ann.write_mann(out_dir);
                    }else if(flags::sequence){

                        vector<am::Image> images;

                        int event_id=get_event_id(ann);

                        if(flags::align){
                            images=ann.align_sequence(event_id);
                        }else{
                            for(int fr=0;fr<ann.events[event_id].frames.size();fr++){
                                images.push_back(ann.images[ann.events[event_id].frames[fr].image_ref]);
                            }
                        }

                        if(flags::offset.arg){
                            for(am::Image&image:images)
                                image.align(image.position_x+image.width+flags::offset.arg,
                                            image.position_y+image.height+flags::offset.arg,
                                            image.position_x-flags::offset.arg,
                                            image.position_y-flags::offset.arg);
                        }

                        int pad_len=get_pad_len(ann.events[event_id].frames.size());

                        for(int fr=0;fr<ann.events[event_id].frames.size();fr++){

                            string number;
                            if(flags::pad)
                                number=pad_int(fr,pad_len);
                            else
                                number=to_string(fr);

                            images[fr].write_png(out_dir+ann.events[event_id].name+"_"+number+".png");
                        }
                    }else{

                        int pad_len=get_pad_len(ann.images.size());

                        for(int im=0;im<ann.images.size();im++){

                            string number;
                            if(flags::pad)
                                number=pad_int(im,pad_len);
                            else
                                number=to_string(im);

                            ann.images[im].write_png(out_dir+ann.name+"_"+number+".png");
                        }
                    }
                }else if(what==code_ann){
                    am::ANN ann(get_file_name(filename));
                    ann.read_any(filename);
                    if(flags::align){
                        for(am::Image& image : ann.images){
                            image.dealign();
                        }
                    }
                    ann.write_ann(out_dir+get_file_name(filename)+".ann");
                }else if(what==decode_img){
                    am::Image image;
                    image.read_img(filename);
                    if(flags::align){
                        image.align();
                    }
                    if(flags::offset.arg){
                        image.align(image.position_x+image.width+flags::offset.arg,
                                    image.position_y+image.height+flags::offset.arg,
                                    image.position_x-flags::offset.arg,
                                    image.position_y-flags::offset.arg);
                    }
                    if(flags::metafile){
                        //image.write_mann(out_dir);
                    }
                    image.write_png(out_dir+get_file_name(filename)+".png");
                }else if(what==code_img){
                    am::Image image;
                    image.read_png(filename);
                    if(flags::align){
                        image.dealign();
                    }
                    image.write_img(out_dir+get_file_name(filename)+".img");
                }
            }catch(...){
                if(flags::ignore==false){
                    cout<<"Can't process: "<<filename<<endl;
                    cout<<"Please contact developer if you think it's program's error."<<endl;
                    cout<<"Continue with further files? [y/n]"<<endl;
                    char yes;
                    cin>>yes;
                    if(yes!='y')
                        return -1;
                }
            }

        }
        cout<<"No more files to process"<<endl;
    }else{
        cout<<"No files to process"<<endl;
        help_page();
    }
    return 0;
}


