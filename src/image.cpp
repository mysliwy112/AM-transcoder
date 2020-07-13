#include<iostream>

#include "am_utils.h"
#include "PNGformat.h"
#include "filesys.h"
#include "CLZW.h"
#include "CRLE.h"

#include "Image.h"

using namespace std;


namespace am{
    Image::Image(string name)
    {
        this->name=name;
    }

    void Image::load_ann(bytes::iterator &offset)
    {
        width=get_int(offset,0x2);
        height=get_int(offset,0x2);
        position_x=get_int(offset,0x2);
        position_y=get_int(offset,0x2);
        compression=get_int(offset,0x2);
        image_size=get_int(offset,0x4);

        advance(offset,0x4);
        advance(offset,0xA);

        alpha_size=get_int(offset,0x4);

        name=get_str(offset,0x14);
        log();
    }

    void Image::log(){
        if(LOG){
            cout<<"Image: "<<name<<endl;
            cout<<"x:"<<position_x<<" y:"<<position_y<<endl;
            cout<<"width:"<<width<<" height:"<<height<<endl;
            cout<<"Compression: "<<compression<<endl;
            cout<<"image_size: "<<image_size<<endl;
            cout<<"alpha_size: "<<alpha_size<<endl;
            cout<<endl;
        }
    }


    dic Image::load_mann(stringstream &offset){
        dic dict;
        while(1){
            dict=get_val(offset);
            if(dict.key=="name"){
                name=dict.value;
            }else if(dict.key=="position_x"){
                position_x=stoi(dict.value);
            }else if(dict.key=="position_y"){
                position_y=stoi(dict.value);
            }else if(dict.key=="compression"){
                compression=stoi(dict.value);
            }else{
                log();
                return dict;
            }
        }

        return dict;
    }

    dic Image::load_mimg(stringstream &offset){
        dic dict;
        string check;
        offset>>check;

        if(LOG){
            if(check.compare("IMG")!=0){
                cout<<"Warning: Inappropriate check string"<<endl;
            }else{
                cout<<"It's meta img file!"<<endl;
            }
        }

        while(1){
            dict=get_val(offset);
            if(dict.key=="name"){
                name=dict.value;
            }else if(dict.key=="image"){
                if(name.size()==0)
                    name=get_file_name(dict.value);
                Image read=read_PNG(dict.value);
                if(width==0)
                    width=read.width;
                if(height==0)
                    height=read.height;
                rgba32=read.rgba32;
            }else if(dict.key=="position_x"){
                position_x=stoi(dict.value);
            }else if(dict.key=="position_y"){
                position_y=stoi(dict.value);
            }else if(dict.key=="compression"){
                compression=stoi(dict.value);
            }else if(dict.key=="width"){
                width=stoi(dict.value);
            }else if(dict.key=="height"){
                height=stoi(dict.value);
            }
            if(offset.eof()){
                log();
                return dict;
            }
        }

        return dict;
    }

    void Image::load_mimg(bytes data){
        stringstream offset(string((char*)data.data(),data.size()));
        vector<string>file;
        load_mimg(offset);
    }



    void Image::load_img(bytes::iterator &offset)
    {
        if(get_str(offset,0x4)=="PIK"&&LOG){
            cout<<"This is img."<<endl;
        }
        width=get_int(offset,0x4);
        height=get_int(offset,0x4);
        bpp=get_int(offset,0x4);
        image_size=get_int(offset,0x4);

        advance(offset,0x4);

        int comp=get_int(offset,0x4);
        if(comp==4)
            comp=0;
        compression=comp;
        alpha_size=get_int(offset,0x4);
        position_x=get_int(offset,0x4);
        position_y=get_int(offset,0x4);
        log();
    }

    void Image::load_data(bytes data)
    {
        image_data img;
        if(data.size()==image_size+alpha_size){
            img.image.assign(data.begin(),data.begin()+image_size);
            img.alpha.assign(data.begin()+image_size,data.end());
        }
        if(compression!=jpeg){
            img.image=decompress(img.image,compression,2);
            img.alpha=decompress(img.alpha,compression,1);
            create_rgba32(img);
        }else{
            img.image=decompress(img.image,compression,2);
            add_alpha(img);
        }

    }

    void Image::load_rgba32(bytes data){
        rgba32=data;
    }

    bytes Image::get_ann_header(int isize,int asize){
        bytes data;

        back_insert_iterator<bytes> offset(data);

        set_int(offset,width,0x2);
        set_int(offset,height,0x2);

        set_int(offset,position_x,0x2);
        set_int(offset,position_y,0x2);

        set_int(offset,compression,0x2);

        set_int(offset,isize,0x4);

        set_data(offset,bytes(1,4));
        set_data(offset,bytes(3,0));
        set_data(offset,bytes(0xA,0));

        set_int(offset,asize,0x4);
        set_str(offset,name,0x14);

        return data;

    }

    bytes Image::get_img_header(int isize,int asize){
        bytes data;

        back_insert_iterator<bytes> offset(data);

        set_str(offset,"PIK",0x4);

        set_int(offset,width,0x4);
        set_int(offset,height,0x4);

        set_int(offset,bpp,0x4);
        set_int(offset,isize,0x4);
        set_data(offset,bytes(4,0));
        set_int(offset,compression,0x4);
        set_int(offset,asize,0x4);

        set_int(offset,position_x,0x4);
        set_int(offset,position_y,0x4);

        return data;
    }

    image_data Image::get_am_data(){

        image_data img;
        img=split_rgba32();

        img.image=compress(img.image,compression,2);
        img.alpha=compress(img.alpha,compression,1);

        return img;
    }

    image_data Image::get_ann(bool doimages){
        image_data data=get_am_data();
        data.header=get_ann_header(data.image.size(),data.alpha.size());
        return data;
    }

    void Image::get_mann(ostringstream &offset,string &file, bool doimages, bool full){
        offset<<"image="<<file<<endl;
        if(full)
            offset<<"\tname="<<name<<endl;
        if(position_x!=0)
            offset<<"\tposition_x="<<position_x<<endl;
        if(position_y!=0)
            offset<<"\tposition_y="<<position_y<<endl;
        if(full)
            offset<<"\twidth="<<width<<endl;
        if(full)
            offset<<"\theight="<<height<<endl;
        if(full)
            offset<<"\tcompression="<<compression<<endl;
    }

    void Image::get_mimg(ostringstream &offset,string &file, bool doimages, bool full){
        offset<<"IMG"<<endl<<endl;
        if(doimages)
            offset<<"image="<<file<<endl;
        if(full)
            offset<<"name="<<name<<endl;
        if(full)
            offset<<"bpp="<<bpp<<endl;
        if(position_x!=0||full)
            offset<<"position_x="<<position_x<<endl;
        if(position_y!=0||full)
            offset<<"position_y="<<position_y<<endl;
        if(full)
            offset<<"width="<<width<<endl;
        if(full)
            offset<<"height="<<height<<endl;
        if(full)
            offset<<"compression="<<compression<<endl;
        if(doimages)
            write_png(file);
    }

    bytes Image::get_mimg(string file,bool doimages, bool full){
        ostringstream offset;
        get_mimg(offset,file,doimages,full);
        string data=offset.str();
        return bytes(data.begin(),data.end());
    }

    bytes Image::get_rgba32(){
        return rgba32;
    }

    void Image::read_any(string filename){
        bytes data=read_file(filename);
        mann_dir=get_directory(filename);
        load(data);
    }

    void Image::load(bytes file){
        if(name.size()>0)
            this->name=name;

        int mode=0;

        string check((char*)file.data(),3);
        if(check=="\x89PNG"){
            mode=1;
        }else if(check=="IMG"){
            mode=2;
        }else if(check=="PIK"){
            mode=3;
        }
        if(LOG)
            cout<<"Image mode:"<<mode<<endl;

        if(mode==0){
            throw invalid_argument("Unknown filetype");
        }else if(mode==1){
            load_PNG(file);
        }else if(mode==2){
            load_mimg(file);
        }else if(mode==3){
            load_img(file);
        }
    }



    void Image::read_png(string filename){
        Image image=read_PNG(filename);
        width=image.width;
        height=image.height;
        bpp=16;
        rgba32=image.rgba32;
    }

    void Image::write_png(string filename){
        write_PNG(rgba32,width,height,8,filename);
    }

    void Image::read_img(string filename){
        bytes data=read_file(filename);
        bytes::iterator i=data.begin();
        load_img(get_data(i,0x28));
        load_data(get_data(i,image_size+alpha_size));
    }

    void Image::write_img(string filename, bool doimages){
        bytes data;
        if(compression==3)
            compression=2;
        image_data img=get_am_data();
        img.header=get_img_header(img.image.size(),img.alpha.size());

        data.insert(data.end(),img.header.begin(),img.header.end());
        if(doimages){
            data.insert(data.end(),img.image.begin(),img.image.end());
            data.insert(data.end(),img.alpha.begin(),img.alpha.end());
        }
        write_file(filename,data);
    }

    void Image::write_mimg(std::string filename, bool doimages, bool full){
        write_file(filename+".mimg",get_mimg(filename+".png",doimages,full));
    }

    bytes Image::decompress(bytes data, int type, int size){
        if(data.size()>0){
            try{
                if(type==0){
                }else if(type==clzw){
                    data=decodeCLZW(data);

                }else if(type==crzw){
                    data=decodeCLZW(data);
                    data=decodeCRLE(data,size);

                }else if(type==crle){
                    data=decodeCRLE(data,size);

                }else if(type==jpeg){
                    data=load_JPEG(data).rgba32;


                }else{
                    throw invalid_argument(string("Unknown compression: ")+to_string(type));
                }
            }catch(...){
                cout<<"Can't decompress image, compression type:"<<type<<endl;
                throw"File decompress problem";
            }
        }

        return data;
    }

    bytes Image::compress(bytes data, int type, int size){
        if(data.size()>0){
            try{
                if(type==0){
                }else if(type==2){
                    data=codeCLZW(data);

                }else if(type==3){
                    data=codeCRLE(data,size);
                    data=codeCLZW(data);

                }else if(type==4){
                    data=codeCRLE(data,size);

                }else if(type==5){
                    cout<<"Just don't compress in jpeg. It's bad for quality ;)"<<endl;
                    throw"Bad taste";
                }else{
                    throw invalid_argument(string("Unknown compression: ")+to_string(type));
                }
            }catch(...){
                cout<<"Can't compress image, compression type:"<<type<<endl;
                throw"File decompress problem";
            }
        }


        return data;
    }

    void Image::align(int max_x, int max_y, int min_x, int min_y){
        if(LOG)
            cout<<"Aligning... max>"<<max_x<<" "<<position_x+width<<"<sum max>"<<max_y<<" "<<position_y+height<<"<sum ";
        if(max_x==0)
            max_x=position_x+width;
        if(max_y==0)
            max_y=position_y+height;

        long long new_width=max_x-min_x;
        long long new_height=max_y-min_y;


        int bpp=4;
        bytes data(new_width*new_height*bpp,0);

        new_width=new_width*bpp;


        int shift_x=(position_x-min_x)*bpp;
        int shift_y=position_y-min_y;
        int width_bpp=width*bpp;
        int height_bpp=height*bpp;

        int in=0;
        int out=shift_x+shift_y*new_width;
        while(in<rgba32.size()&&out<data.size()){
            copy(rgba32.begin()+in,rgba32.begin()+in+width_bpp,data.begin()+out);
            in+=width_bpp;
            out+=new_width;
        }
        position_x=min_x;
        position_y=min_y;
        width=new_width/bpp;
        height=new_height;

        rgba32=data;
        if(LOG)
            cout<<"completed"<<endl;
    }

    void Image::dealign(){
        long long up=0;
        long long down=0;
        long long left=0;
        long long right=0;
        long long how=0;
        int bpp=4;

        for(how=bpp-1;up<rgba32.size();how+=bpp){
            if(rgba32[how]!=0){
                up=how/bpp/width;
                break;
            }
        }

        for(how=rgba32.size()-1;how>=0;how-=bpp){
            if(rgba32[how]!=0){
                down=(rgba32.size()-how)/bpp/width;
                break;
            }
        }


        for(how=bpp-1;how!=rgba32.size()-1;how+=width*bpp){
            if(how>rgba32.size()){
                how=how-rgba32.size()+bpp;
            }
            if(rgba32[how]!=0){
                left=(how/bpp)%width;
                break;
            }
        }

        for(how=rgba32.size()-1;how!=bpp;how-=width*bpp){
            if(how<0){
                how=how+rgba32.size()-bpp;
            }
            if(rgba32[how]!=0){
                right=width-(how/bpp)%width;
                break;
            }
        }

        int old_width=width;
        int old_height=height;

        position_y+=up;
        position_x+=left;
        height-=up+down;
        width-=left+right;

        bytes new_rgba32(width*height*bpp,0);
        int point=0;
        while(point<height){
            copy(rgba32.begin()+(up*old_width+left+point*old_width)*bpp,rgba32.begin()+(up*old_width+left+point*old_width+width)*bpp, new_rgba32.begin()+point*width*bpp);
            point++;
        }
        rgba32=new_rgba32;
    }


    void Image::create_rgba32(image_data img){

        bytes n(img.image.size()/2*3);
        int temp;
        int counter=0;
        for(unsigned int i=0;i<img.image.size();i+=2){
                temp=img.image[i]+img.image[i+1]*256;
                n[counter+2]=(temp%32)*256/32;
                temp=(temp-temp%32)/32;
                n[counter+1]=(temp%64)*256/64;
                temp=(temp-temp%64)/64;
                n[counter]=(temp%32)*256/32;
                counter+=3;
        }
        img.image=n;
        add_alpha(img);

    }

    void Image::add_alpha(image_data img){
        rgba32.resize(img.image.size()+img.image.size()/3,0);
        int al=0;
        int da=0;
        for(unsigned int i=0;i<rgba32.size();i+=4){
            rgba32[i]=img.image[da];
            rgba32[i+1]=img.image[da+1];
            rgba32[i+2]=img.image[da+2];
            if(img.alpha.size()==0)
                rgba32[i+3]=255;
            else
                rgba32[i+3]=img.alpha[al];
            da+=3;
            al++;
        }
    }

    image_data Image::split_rgba32(){
        image_data img;
        img.image.resize(rgba32.size()/2,0);
        img.alpha.resize(rgba32.size()/4,0);
        int im_p=0;
        int al_p=0;
        for(int i=0;i<rgba32.size();i+=4){
            img.image[im_p]+=uint8_t(float(rgba32[i+2])*32/256);
            img.image[im_p]+=uint8_t(float(rgba32[i+1])*64/256)<<5;
            img.image[im_p+1]+=uint8_t(float(rgba32[i+1])*64/256)>>3;
            img.image[im_p+1]+=uint8_t(float(rgba32[i])*32/256)<<3;
            img.alpha[al_p]=rgba32[i+3];
            im_p+=2;
            al_p+=1;
        }

        return img;
    }

    void Image::load_img(bytes data){
        bytes::iterator offset=data.begin();
        load_img(offset);
        load_data(get_data(offset,image_size+alpha_size));
    }

};
