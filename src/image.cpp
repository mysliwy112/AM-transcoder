#include<iostream>

#include "am_utils.h"
#include "PNGformat.h"
#include "filesys.h"
#include "CLZW.h"
#include "CRLE.h"

#include "Image.h"

using namespace std;


namespace am{
    Image::Image()
    {
        //ctor
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
            if(dict.key=="position_x"){
                position_x=stoi(dict.value);
            }else if(dict.key=="position_y"){
                position_y=stoi(dict.value);
            }else if(dict.key=="name"){
                name=dict.value;
            }else{
                log();
                return dict;
            }
        }

        return dict;
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
    }

    void Image::load_data(bytes data)
    {
        image_data img;
        if(data.size()==image_size+alpha_size){
            img.image.assign(data.begin(),data.begin()+image_size);
            img.alpha.assign(data.begin()+image_size,data.end());
        }
        img.image=decompress(img.image,compression,2);
        img.alpha=decompress(img.alpha,compression,1);
        compression=0;
        create_rgba32(img);
    }

    void Image::load_rgba32(bytes data){
        rgba32=data;
    }

    bytes Image::get_ann_header(int compression,int isize,int asize){
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

    bytes Image::get_img_header(int compression,int isize,int asize){
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

    image_data Image::get_am_data(int compression){

        image_data img;
        img=split_rgba32();

        img.image=compress(img.image,compression,2);
        img.alpha=compress(img.alpha,compression,1);

        return img;
    }

    image_data Image::get_ann(){
        int comp=4;
        image_data data=get_am_data(comp);
        data.header=get_ann_header(comp,data.image.size(),data.alpha.size());
        return data;
    }

    void Image::get_mann(std::ostringstream &offset,string &file){
        offset<<"image="<<file<<endl;
        if(position_x!=0)
            offset<<"\tposition_x="<<position_x<<endl;
        if(position_y!=0)
            offset<<"\tposition_y="<<position_y<<endl;

    }



    bytes Image::get_rgba32(){
        return rgba32;
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

    void Image::write_img(string filename){
        int cmpr=2;
        bytes data;

        image_data img=get_am_data(cmpr);
        img.header=get_img_header(cmpr,img.image.size(),img.alpha.size());

        data.insert(data.end(),img.header.begin(),img.header.end());
        data.insert(data.end(),img.image.begin(),img.image.end());
        data.insert(data.end(),img.alpha.begin(),img.alpha.end());

        write_file(filename,data);
    }

    bytes Image::decompress(bytes data, int type, int size){
        if(data.size()>0){
            try{
                if(type==0){
                }else if(type==4){
                        data=decodeCRLE(data,size);

                }else if(type==3){
                        data=decodeCLZW(data);
                        data=decodeCRLE(data,size);

                }else if(type==2){
                        data=decodeCLZW(data);

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
                }else if(type==4){
                    data=codeCRLE(data,size);
                }else if(type==3){
                    data=codeCRLE(data,size);
                    data=codeCLZW(data);

                }else if(type==2){
                    data=codeCLZW(data);

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
    }

};
