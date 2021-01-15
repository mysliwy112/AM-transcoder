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
    virtual string get_desc(){return desc;}
    operator bool() const { return v; }

    string name;
    bool shorten;
    string desc;
    bool v;

};

//use when nested flags would be needed
//template <typename T, typename = int>
//struct HasX : std::false_type { };
//
//template <typename T>
//struct HasX <T, decltype((void) T::list, 0)> : std::true_type { };

template <typename arg_type = char>
struct Flag : public Flag_main{
    Flag(vector<Flag_main*> &lis, string name, bool shorten, string desc, bool v=false, arg_type arg='\b'){
        lis.push_back((Flag_main*)this);
        this->name=name;
        this->shorten=shorten;
        this->desc=desc;
        this->v=v;
        this->arg=arg;
    }

    void set_arg(string arg){
        if constexpr(is_arithmetic<arg_type>::value){
            this->arg=stoi(arg);
        //use when nested flags would be needed
        //}else if constexpr(HasX<arg_type>::value){
        //    get_flags_2(this->arg.list,arg);
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

};


struct Flags{
    vector<Flag_main*> list;
    Flag<> help{list,"help",true,"Shows help message.",false};
    Flag<> file_dir{list,"file",true,"Sets output to input file's director.",false};
    Flag<> name_dir{list,"name-off",true,"Stops creation of directory named by input ann's name.",true};
    Flag<string> out_directory{list,"directory",true,"Sets output directory.",false,"./"};
    Flag<int> pad{list,"pad",true,"Pads numbers with zeros to specified number of digits.",false,0};
    Flag<> log{list,"verbose",true,"Generates additional console output, used mainly for debbuging.",false};
    Flag<> align{list,"align",true,"Align image sizes.",false};
    Flag<> ignore{list,"ignore",true,"Ignores errors.",false};
    Flag<string> sequence{list,"sequence",true,"Creates event sequence {type \":\" to get sequence by number}{events names are going to be listed and can be chosen from on runtime}.",false,""};
    //Flag<> sequenced{flags.list,"quences","Extracts every sequence as another folder.",true};
    Flag<int> offset{list,"offset",true,"Adds transparent pixels to all sides of image.",false,10};
    Flag<string> metafile{list,"metafile",true,"Creates Meta file and additional images, used for encoding anns and imgs.",false,""};
    Flag<string> json{list,"json",true,"Creates Meta file in json format.",false,""};
    Flag<> images{list,"images-off",false,"Generates mann without additional image data.",true};
    Flag<> full{list,"full",false,"Fully transcripts ann to mann.",false};
    Flag<> merge{list,"merge",false,"(Experimental) Merges decoded file to destination metafile.",false};
}flags;


void help_page(){
    cout<<"Transcode ANN and IMG";
    cout<<endl<<endl;
    int namelong=0;
    for(Flag_main* &flag:flags.list){
        if(flag->name.size()>namelong)
            namelong=flag->name.size()+flag->get_arg().size();
    }
    for(Flag_main* &flag:flags.list){
        if(flag->shorten)
            cout<<"-"<<flag->name[0]<<" ";
        else{
            cout<<"--";
        }
        cout<<flag->name<<"="<<flag->get_arg()<<setw(namelong+1+flag->desc.size()-flag->name.size())<<flag->desc<<endl;
    }
}

bool get_flags(string command){
    char delim='=';
    if(command[1]=='-'){
        command=command.substr(2);
        int i=0;
        for(Flag_main* &flag:flags.list){
            if(command.compare(0,command.find_first_of(delim),flag->name)==0){
                flag->v=!flag->v;
                if(command.find_first_of(delim)!=string::npos){
                    flag->set_arg(command.substr(command.find_first_of(delim)+1));
                }
                break;
            }
            i++;
        }
        if(i==flags.list.size()){
            return false;
            cout<<"Unknown flag "<<command<<endl;
            throw "Unknown flag";
        }
    }else{
        command=command.substr(1);
        int part=0;
        while(command[part]!=delim&&part<command.size()){
            int i=0;
            for(Flag_main* &flag:flags.list){
                if(flag->shorten==true&&flag->name[0]==command[part]){
                    flag->v=!flag->v;
                    if(part+1<command.size()&&command[part+1]==delim){
                        flag->set_arg(command.substr(part+2));
                        part=command.size();
                    }
                    break;
                }
                i++;
            }

            if(i==flags.list.size()){
                return false;
                cout<<"Unknown flag "<<command[part]<<endl;
                throw "Unknown flag";
            }
            part++;
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
    if(flags.log)
        cout<<"Magic: "<<check<<endl;
    if(check.substr(0,3)=="NVP"||check.substr(0,3)=="NVM")
        return 0;
    else if(check.substr(0,3)=="ANN"||check.substr(0,1)=="{")
        return 1;
    else if(check.substr(0,3)=="PIK")
        return 2;
    else if(check=="\x89PNG"||check.substr(0,3)=="IMG")
        return 3;
    return -1;
}

int get_events_ids(am::ANN &ann){
    string sequence=flags.sequence.arg;
    try{
        if(sequence.size()==0){
            cout<<endl;
            for(int ev=0;ev<ann.events.size();ev++){
                cout<<ev<<".\t"<<ann.events[ev].name<<endl;
            }
            cout<<"Choose event name: (type \":\") to get sequence by number"<<endl;
            cin>>sequence;
        }


        if(sequence.compare(0,1,":")==0){
            return stoi(sequence.substr(1));
        }else{
            return ann.get_event_index(sequence);
        }
    }catch(...){
        string error=string("Can't find event with name |")+sequence+"|";
        cout<<error<<endl;
        throw invalid_argument(error);
    }

}

int main(int argc, char *argv[])
{
    if(argc>1){
        parse_commandline(argv,argc);
        if(flags.help){
            help_page();
            return 1;
        }
        if(flags.pad)
            am::PAD=flags.pad.arg;
        am::LOG=flags.log;

        for(string &filename : filenames){
//            try{
                string out_dir;

                if(flags.file_dir){
                    out_dir=get_directory(filename);
                }else if(flags.out_directory){
                    out_dir=flags.out_directory.arg;
                }else{
                    out_dir=get_directory(argv[0]);
                }

                int what=get_mode(read_file(filename,4));

                if(flags.log)
                    cout<<"Mode: "<<what<<endl;

                if(flags.name_dir&&what==decode_ann){
                    out_dir+=get_file_name(filename)+string("/");
                }

                create_directory(out_dir);

                if(flags.log)
                    cout<<"Out directory: "<<out_dir<<endl;

                if(what==decode_ann){
                    am::ANN ann(get_file_name(filename));
                    ann.read_any(filename);

                    if(flags.merge){
                        if(flags.metafile){
                            try{
                                ann.read_any(out_dir+ann.name+".mann");
                            }catch(...){
                                if(flags.log)
                                    cout<<"Can't read file to merge."<<endl;
                            }
                            if(flags.json){
                                cout<<"Warning: Json file merges as second one"<<endl;
                            }
                        }
                        if(flags.json){
                            try{
                                ann.read_any(out_dir+ann.name+".mann");
                            }catch(...){
                                if(flags.log)
                                    cout<<"Can't read file to merge."<<endl;
                            }
                        }
                    }

                    if(flags.align&&!flags.sequence){
                        if(flags.log)
                            cout<<"File align"<<endl;
                        ann.align_images();
                        if(flags.log)
                            cout<<"completed"<<endl;
                    }

                    if(flags.offset&&!flags.sequence){
                        for(am::Image&image:ann.images)
                            image.add_align(image.position_x+image.width+flags.offset.arg,
                                        image.position_y+image.height+flags.offset.arg,
                                        image.position_x-flags.offset.arg,
                                        image.position_y-flags.offset.arg);

                    }


                    if(flags.metafile||flags.json){
                        for(am::Image&image:ann.images)
                            image.align();
                        if(flags.metafile){
                            if(flags.metafile.arg.size()>0)
                                ann.name=flags.metafile.arg;
                            ann.write_mann(out_dir,flags.images,flags.full);
                        }
                        if(flags.json){
                            if(flags.json.arg.size()>0)
                                ann.name=flags.json.arg;
                            ann.write_jann(out_dir,flags.images,flags.full);
                        }
                    }else if(flags.sequence){
                        if(flags.images){
                            vector<am::Image> images;

                            int event_id=get_events_ids(ann);


                            for(int fr=0;fr<ann.events[event_id].frames.size();fr++){
                                images.push_back(ann.images[ann.events[event_id].frames[fr].image_ref]);
                            }

                            if(flags.align){
                                ann.align_sequence(images,event_id);
                            }

                            if(flags.offset.arg){
                                for(am::Image&image:images)
                                    image.add_align(image.position_x+image.width+flags.offset.arg,
                                                image.position_y+image.height+flags.offset.arg,
                                                image.position_x-flags.offset.arg,
                                                image.position_y-flags.offset.arg);
                            }


                            int pad_len=get_pad_len(ann.images.size());
                            for(int fr=ann.events[event_id].frames.size()-1;fr>=0;fr--){

                                string number;
                                if(flags.pad)
                                    number=pad_int(fr,pad_len);
                                else
                                    number=to_string(fr);

                                images[fr].align();
                                images[fr].write_png(out_dir+ann.events[event_id].name+"_"+number+".png");
                                images.pop_back();
                            }
                        }
                    }else{
                        if(flags.images){
                            int pad_len=get_pad_len(ann.images.size());

                            for(int im=ann.images.size()-1;im>=0;im--){
                                string number;
                                if(flags.pad)
                                    number=pad_int(im,pad_len);
                                else
                                    number=to_string(im);

                                ann.images[im].align();
                                ann.images[im].write_png(out_dir+ann.name+"_"+number+".png");
                                ann.images.pop_back();
                            }
                        }
                    }
                }else if(what==code_ann){
                    am::ANN ann(get_file_name(filename));
                    ann.read_any(filename);
                    if(flags.align){
                        for(am::Image& image : ann.images){
                            image.dealign();
                        }
                    }
                    ann.write_ann(out_dir+ann.name+".ann",flags.images);
                }else if(what==decode_img){
                    am::Image image(get_file_name(filename));
                    image.read_any(filename);

                    if(flags.merge){
                        if(flags.metafile){
                            try{
                                image.read_any(out_dir+image.name+".mimg");
                            }catch(...){
                                if(flags.log)
                                    cout<<"Can't read file to merge."<<endl;
                            }
                            if(flags.json){
                                cout<<"Warning: Json is merged as second file"<<endl;
                            }
                        }
                        if(flags.json){
                            try{
                                image.read_any(out_dir+image.name+".jimg");
                            }catch(...){
                                if(flags.log)
                                    cout<<"Can't read file to merge."<<endl;
                            }
                        }

                    }

                    if(flags.align){
                        image.add_align();
                    }
                    if(flags.offset){
                        image.add_align(image.position_x+image.width+flags.offset.arg,
                                    image.position_y+image.height+flags.offset.arg,
                                    image.position_x-flags.offset.arg,
                                    image.position_y-flags.offset.arg);
                    }

                    image.align();


                    if(flags.metafile||flags.json){
                        if(flags.metafile){
                            image.write_mimg(out_dir+image.name,flags.images,flags.full);
                        }
                        if(flags.json){
                            image.write_jimg(out_dir+image.name,flags.images&&(!flags.metafile),flags.full);
                        }
                    }else if(flags.images){
                        image.write_png(out_dir+image.name+".png");
                    }
                }else if(what==code_img){
                    am::Image image(get_file_name(filename));
                    image.read_any(filename);
                    if(flags.align){
                        image.dealign();
                    }
                    image.write_img(out_dir+image.name+".img",flags.images);
                }
//            }catch(...){
//                if(flags.ignore==false){
//                    cout<<"Can't process: "<<filename<<endl;
//                    cout<<"Please contact developer if you think it's program's error."<<endl;
//                    cout<<"Continue with further files? [y/n] ";
//                    char yes;
//                    cin>>yes;
//                    if(yes!='y')
//                        return -1;
//                }
//            }

        }
        cout<<"No more files to process"<<endl;
        system("pause");
    }else{
        cout<<"No files to process"<<endl;
        help_page();
    }
    return 0;
}


