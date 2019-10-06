#ifndef ANN_H
#define ANN_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdexcept>
#include<algorithm>

#include<PNGformat.h>
#include<CLZW.h>
#include<CRLE.h>
#include<filesys.h>

using namespace std;

    struct image{
        unsigned short width;
        unsigned short height;
        short position_x;
        short position_y;
        unsigned short compression;
        unsigned int image_size;
        //int nwm1;
        //string nwm2;
        unsigned int alpha_size;
        string name;
        vector<unsigned char> image_data;
        vector<unsigned char> alpha_data;
        vector<unsigned char> decode_data;
    };

    struct frame{
        unsigned short image_ref;
        string check;
        //int nwm1;
        short position_x;
        short position_y;
        unsigned int sfx_switch;
        //int nwm2;
        uint8_t transparency;
        //string nwm3;
        string name;
        string sounds;
    };

    struct event{
        string name;
        unsigned int frames_number;
        //int nwm1;
        unsigned int loop_number;
        //int nwm2;
        unsigned int transparency;
        //int nwm3;
        vector<frame> frames;
    };

    struct header{
        string name;
        string check;
        unsigned short images_number;
        unsigned short bpp;
        unsigned short events_number;
        //string opis1;
        //int8_t opis2;
        //string opis3;
        int transparency;
        //vector<char> dump;
        unsigned int caption_size;
        string caption;
    };

class ANN
{
    public:
        ANN(){}

        header head;
        string name;
        vector<event> events;
        vector<image> images;

        bool is_seqence=false;
        string seqence_name;

        bool align=false;

        bool to_raw=false;

        bool log=false;


        bool is_bigfile=false;

        void set_align(bool a){
            align=a;
        }

        void set_raw(bool a){
            to_raw=a;
        }

        void set_log(bool a){
            log=a;
        }

        void set_sequence(bool is, string s={}){
            is_seqence=is;
            seqence_name=s;
        }

        void load_ann(string filename){
                vector<unsigned char> file;

                ifstream bigfile;

                try{
                    file=read_file(filename);
                }catch(const invalid_argument& e){
                    cout<<e.what()<<endl;
                    throw;
                }

                if(file.size()==0){
                    is_bigfile=true;
                    if(log){
                        cout<<"BIGFILE it is"<<endl;
                    }
                }

                if(is_bigfile){
                    bigfile.open(filename,ios::binary);
                }

                if(is_bigfile){
                    file=read_file(bigfile,0x30);
                }

                head.check.assign((char*)file.data(),3);
                if(head.check.compare("NVP")==0){
                    if(log)
                        cout<<"Yes, this is ANN!"<<endl;
                }else{
                    throw invalid_argument("Sorry but this is probably not ANN.");
                }

                name=get_file_name(filename);





                //load header
                head.images_number=combine(file.data()+0x4,0x2);
                head.bpp=combine(file.data()+0x6,0x2);
                head.events_number=combine(file.data()+0x8,0x2);
                //head.opis1.assign((char*)file.data()+0xA,0xD);
                //head.opis2=combine(file.data()+0x17,0x2);
                //head.opis3.assign((char*)file.data()+0x1B,0x4);
                head.transparency=combine(file.data()+0x1F,0x1);
                //head.dump.assign((char*)file.data()+0x13,0x9);
                head.caption_size=combine(file.data()+0x2C,0x4);

                if(is_bigfile){

                    file=read_file(bigfile,head.caption_size+0x4);
                    head.caption.assign((char*)file.data(),head.caption_size);
                }else{
                    head.caption.assign((char*)file.data()+0x30,head.caption_size);
                }
                events.resize(head.events_number);
                unsigned long long offset=0;

                if(!is_bigfile)
                    offset=0x34+head.caption_size;


                for(int ev=0;ev<head.events_number;ev++){

                    if(is_bigfile)
                        file=read_file(bigfile,0x43);

                    events[ev].name=string((char*)file.data()+offset);
                    events[ev].frames_number=combine(file.data()+offset+0x20,0x2);
                    events[ev].loop_number=combine(file.data()+offset+0x28,0x4);
                    events[ev].transparency=combine(file.data()+offset+0x36,0x1);
                    events[ev].frames.resize(events[ev].frames_number);

                    if(is_bigfile)
                        file=read_file(bigfile,events[ev].frames_number*2);
                    else
                        offset+=0x43;

                    for(unsigned int fr=0;fr<events[ev].frames_number;fr++){
                        events[ev].frames[fr].image_ref=combine(file.data()+offset,0x2);
                        offset+=2;
                    }



                    for(unsigned int fr=0;fr<events[ev].frames_number;fr++){

                        if(is_bigfile){
                            offset=0;
                            file=read_file(bigfile,0x22);
                        }

                        events[ev].frames[fr].check.assign((char*)file.data()+offset,0x4);
                        events[ev].frames[fr].position_x=combine(file.data()+offset+0x8,0x2);
                        events[ev].frames[fr].position_y=combine(file.data()+offset+0xA,0x2);
                        events[ev].frames[fr].sfx_switch=combine(file.data()+offset+0x10,0x4);
                        events[ev].frames[fr].transparency=combine(file.data()+offset+0x18,0x1);

                        if(is_bigfile){
                            int nam_size=combine(file.data()+offset+0x1E,0x4);
                            file=read_file(bigfile,nam_size);
                            events[ev].frames[fr].name.assign((char*)file.data(),nam_size);
                        }else{
                            events[ev].frames[fr].name.assign((char*)file.data()+offset+0x22,combine(file.data()+offset+0x1E,0x4));
                            offset+=0x22+events[ev].frames[fr].name.size();
                        }



                        if(events[ev].frames[fr].sfx_switch!=0){
                            if(is_bigfile){
                                file=read_file(bigfile,0x4);
                                int nam_size=combine(file.data(),0x4);
                                file=read_file(bigfile,nam_size);
                                events[ev].frames[fr].sounds.assign((char*)file.data(),nam_size);
                            }else{
                                events[ev].frames[fr].sounds.assign((char*)file.data()+offset+0x4,combine(file.data()+offset,0x4));
                                offset+=0x4+events[ev].frames[fr].sounds.size();
                            }
                        }
                    }
                }

                images.resize(head.images_number);



                for(int im=0;im<head.images_number;im++){

                    if(is_bigfile){
                        offset=0;
                        file=read_file(bigfile,0x34);
                    }

                    images[im].width=combine(file.data()+offset,0x2);
                    images[im].height=combine(file.data()+offset+0x2,0x2);
                    images[im].position_x=combine(file.data()+offset+0x4,0x2);
                    images[im].position_y=combine(file.data()+offset+0x6,0x2);
                    images[im].compression=combine(file.data()+offset+0x8,0x2);
                    images[im].image_size=combine(file.data()+offset+0xA,0x4);
                    images[im].alpha_size=combine(file.data()+offset+0x1C,0x4);
                    images[im].name=string((char*)file.data()+offset+0x20);
                    if(!is_bigfile)
                        offset+=0x34;
                }


                for(int im=0;im<head.images_number;im++){
                    if(is_bigfile){

                        file=read_file(bigfile,images[im].image_size);
                    }

                    images[im].image_data.assign(file.begin()+offset,file.begin()+offset+images[im].image_size);

                    if(is_bigfile){
                        file=read_file(bigfile,images[im].alpha_size);
                    }else{
                        offset+=images[im].image_size;
                    }

                    images[im].alpha_data.assign(file.begin()+offset,file.begin()+offset+images[im].alpha_size);
                    if(!is_bigfile){
                        offset+=images[im].alpha_size;
                    }
                }

                if(log==1){
                    cout<<"HEADER:"<<endl;
                    cout<<"\tNumber of events: "<<head.events_number<<endl;
                    cout<<"\tNumber of images: "<<head.images_number<<endl;
                    cout<<"\tTHE CREATOR: "<<head.caption<<endl;
                    for(int ev=0;ev<head.events_number;ev++){
                        cout<<"EVENT:"<<endl;
                        cout<<"\tName: "<<events[ev].name<<endl;
                        cout<<"\tNumber of frames: "<<events[ev].frames_number<<endl;
                        for(int fr=0;fr<events[ev].frames_number;fr++){
                            cout<<"\tFRAME:"<<endl;
                            cout<<"\tFrame name: "<<events[ev].frames[fr].name<<endl;
                            cout<<"\t\tImage number: "<<events[ev].frames[fr].image_ref<<endl;
                        }
                    }
                    for(int im=0;im<head.images_number;im++){
                        cout<<"IMAGE:"<<endl;
                        cout<<"\tImage name: "<<images[im].name<<endl;
                        cout<<"\tImage size: "<<images[im].image_size<<endl;
                    }
                    if(file.size()==offset)
                        cout<<"Size of file: "<<offset<<endl;
                    else
                        cout<<"Size of file and data doesn't match: file->"<<file.size()<<" <> "<<offset<<"<-offset"<<endl;
                }

//            }catch(const invalid_argument& e){
//                cout<<e.what()<<endl;
//                throw;
//            }
        }

        void extract_ANN(string directory){
            if(log){
                cout<<"I'm extracting"<<endl;
            }
            //for images aligning
            int max_x=0;
            int max_y=0;
            int min_x=0;
            int min_y=0;

            if(is_seqence){
                int ev_id;
                try{
                    ev_id=get_event_id(seqence_name);
                }catch(invalid_argument &e){
                    cout<<e.what()<<endl;
                    throw;
                }
                if(align){
                    for(int fr=0;fr<events[ev_id].frames_number;fr++){
                        int im=events[ev_id].frames[fr].image_ref;
                        if(images[im].decode_data.size()==0){
                            decodeImage(images[im]);

                            //cout<<im<<endl;

                            if(images[im].position_x+images[im].width>max_x){
                                max_x=images[im].position_x+images[im].width;
                            }
                            if(images[im].position_y+images[im].height>max_y){
                                max_y=images[im].position_y+images[im].height;
                            }
                            if(images[im].position_x<0){
                                if(images[im].position_x*-1>min_x){
                                    min_x=images[im].position_x*-1;
                                }
                            }
                            if(images[im].position_y<0){
                                if(images[im].position_y*-1>min_y){
                                    min_y=images[im].position_y*-1;
                                }
                            }
                        }
                    }
                }

                int dig=pad_int(events[ev_id].frames_number);


                for(int fr=0;fr<events[ev_id].frames_number;fr++){
                    int im=events[ev_id].frames[fr].image_ref;

                    if(align){
                        if(images[im].decode_data.size()<max_x*max_y*4)
                            align_image(images[im],max_x,max_y,min_x,min_y);
                    }else{
                        if(images[im].decode_data.size()==0)
                            decodeImage(images[im]);
                    }

                    int fr_dig=pad_int(fr);

                    string filename;
                    filename=directory+name+string("_")+events[ev_id].name+string("_")+string(dig-fr_dig,'0')+to_string(fr)+string(".png");
                    if(to_raw)
                        extract_to_file(images[im].decode_data,filename);
                    else
                        extract_to_png(images[im],filename.c_str());
                }


            }else{
                if(align){
                    for(int im=0;im<head.images_number;im++){
                        decodeImage(images[im]);
                        if(images[im].position_x+images[im].width>max_x){
                            max_x=images[im].position_x+images[im].width;
                        }
                        if(images[im].position_y+images[im].height>max_y){
                            max_y=images[im].position_y+images[im].height;
                        }
                        if(images[im].position_x<0){
                            if(images[im].position_x*-1>min_x){
                                min_x=images[im].position_x*-1;
                            }
                        }
                        if(images[im].position_y<0){
                            if(images[im].position_y*-1>min_y){
                                min_y=images[im].position_y*-1;
                            }
                        }
                    }
                }

                for(int im=0;im<head.images_number;im++){

                    if(align)
                        align_image(images[im],max_x,max_y,min_x,min_y);
                    else
                        decodeImage(images[im]);
                    string filename;

                    filename=directory+name+string("_")+images[im].name+string("_")+to_string(im)+string(".png");
                    if(to_raw)
                        extract_to_file(images[im].decode_data,filename);
                    else
                        extract_to_png(images[im],filename.c_str());
                }
            }

        }

        int pad_int(int number){
            int digits = 0;
            if (number < 0) digits = 1;
            if(number==0) return 1;
            while (number) {
                number /= 10;
                digits++;
            }
            return digits;
        }

        void align_image(image& img, int max_x, int max_y, int min_x, int min_y, int bpp=4){
            int pos=0;
            int end_add=max_x-img.position_x-img.width;
            img.decode_data.insert(img.decode_data.begin(),(img.position_y+min_y)*max_x*4,0);
            pos+=(img.position_y+min_y)*max_x*4;
            while(pos<img.decode_data.size()){
                img.decode_data.insert(img.decode_data.begin()+pos,(img.position_x+min_x)*bpp,0);
                pos+=(img.position_x+min_x)*bpp+img.width*bpp;
                img.decode_data.insert(img.decode_data.begin()+pos,end_add*bpp,0);
                pos+=end_add*bpp;
            }
            img.decode_data.insert(img.decode_data.end(),(max_y-img.position_y-img.height)*max_x*bpp,0);
            img.width=max_x;
            img.height=max_y;
        }

        void decodeImage(image &img){
            vector<unsigned char> color;
            vector<unsigned char> alpha;

            if(log){
                cout<<"I'm decoding"<<endl;
            }

            if(img.compression==4){
                color=decodeCRLE(img.image_data,2);
                alpha=decodeCRLE(img.alpha_data,1);
            }
            if(img.compression==3){
                color=decodeCLZW(img.image_data);
                alpha=decodeCLZW(img.alpha_data);
                color=decodeCRLE(color,2);
                alpha=decodeCRLE(alpha,1);
            }
            color=to_24bpp(color);
            img.decode_data=link(color, alpha);

        }

        void extract_to_png(image img, string filename){
                write_PNG(img.decode_data.data(),img.width,img.height,8,filename.c_str());
        }
        void extract_to_file(vector<unsigned char> code,string filename){
            fstream file(filename,ios::out|ios::binary);
            file.write((char*)code.data(),code.size());
            file.close();
        }

    protected:

        vector<unsigned char> read_file(string filename){
            ifstream file(filename,ios::binary);
            if(!file.good()){
                throw invalid_argument(string("Can't access file.")+filename);
            }
            file.seekg (0, ios::end);
            int f_size=file.tellg();
            file.seekg (0, ios::beg);
            if(f_size>2000000){
                file.close();
                return vector<unsigned char>(0);
            }
            char buffer[f_size];

            file.read(buffer,f_size);
            file.close();
            return vector<unsigned char>(buffer,f_size+buffer);
        }

        vector<unsigned char> read_file(ifstream &file, long long f_size){
            char buffer[f_size];
            file.read(buffer,f_size);
            return vector<unsigned char>(buffer,f_size+buffer);
        }

        unsigned long long combine(unsigned char* value,int number){
            unsigned long long out=0;
            unsigned long long mult=1;
            for(int i=0;i<number;i++){
                out+=(unsigned long long)value[i]*mult;
                mult*=256;
            }
            return out;
        }

        vector<unsigned char> link(vector<unsigned char> color, vector<unsigned char> alpha){
            vector<unsigned char> data(color.size()+alpha.size(),0);
            //cout<<data.size()<<endl;
            //cout<<float(data.size())/4<<endl;
            int al=0;
            int da=0;
            for(int i=0;i<data.size();i+=4){
                data[i]=color[da];
                data[i+1]=color[da+1];
                data[i+2]=color[da+2];
                data[i+3]=alpha[al];
                da+=3;
                al++;
            }
            return data;
        }
        vector<unsigned char> to_24bpp(vector<unsigned char> color){
            vector<unsigned char> n(color.size()/2*3);
            int temp;
            int counter=0;
            for(int i=0;i<color.size();i+=2){
                    temp=color[i]+color[i+1]*256;
                    n[counter+2]=(temp%32)*256/32;
                    temp=(temp-temp%32)/32;
                    n[counter+1]=(temp%64)*256/64;
                    temp=(temp-temp%64)/64;
                    n[counter]=(temp%32)*256/32;
                    counter+=3;
            }
            return n;
        }

        int get_event_id(string name){
            if(name.size()==0){
                cout<<endl;
                for(int ev=0;ev<head.events_number;ev++){
                    cout<<ev+1<<".\t"<<events[ev].name<<endl;
                }
                cout<<"Choose event name: (type \"id:\") to get by id"<<endl;
                cin>>name;
            }
            if(name.compare(0,3,"id:")==0){
                return stoi(name.substr(3))-1;
            }else{
                for(int ev=0;ev<head.events_number;ev++){
                    if(name.compare(events[ev].name)==0){
                        return ev;
                    }
                }
            }

            throw invalid_argument(string("Can't find event with name: ")+name);

        }
    private:
};

#endif // ANN_H
