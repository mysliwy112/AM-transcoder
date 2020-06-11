#include "am_utils.h"
#include <iostream>
#include <sstream>


using namespace std;

namespace am{
    extern bool LOG=false;
    extern int PAD=-1;
    Graphics::Graphics(){}

    long long Graphics::get_int(bytes::iterator &offset,int length){

        unsigned long long out=0;
        int mult=1;
        for(int i=0;i<length;i++){
            out+=*(offset+i)*mult;
            mult*=256;
        }
        advance(offset,length);

        long long var=(long long)out;

        return var;
    }


    void Graphics::set_int(back_insert_iterator<bytes> &offset,unsigned long long number,int length){
        bytes data(length);
        for (int i = 0; i < length; i++){
            data[i] = (number >> (i * 8));
        }
        copy(data.begin(),data.end(),offset);

    }

    string Graphics::get_str(bytes::iterator &offset,int length){
        string str(offset,offset+length);
        string out(str.c_str());
        advance(offset,length);
        return out;
    }

    void Graphics::set_str(back_insert_iterator<bytes> &offset, std::string str, int length){
        bytes dif;

        if(length==0){
            length=str.size();
        }else if(length>str.size()){
            dif.resize(length-str.size(),0);
            length=str.size();
        }
        copy(str.begin(),str.begin()+length,offset);
        copy(dif.begin(),dif.end(),offset);

    }

    bytes Graphics::get_data(bytes::iterator &offset,int length){
        bytes data(offset,offset+length);
        advance(offset,length);
        return data;
    }

    void Graphics::set_data(back_insert_iterator<bytes> &offset,bytes data){
        copy(data.begin(),data.end(),offset);
    }

    void Graphics::check_bound(bytes::iterator offset,unsigned long long length){
        throw "Pointer out of bounds";
    }

    unsigned long long Graphics::combine(bytes::iterator value,int siz){
        unsigned long long out=0;
        int mult=1;
        for(int i=0;i<siz;i++){
            out+=*(value+i)*mult;
            mult*=256;
        }
        return out;
    }

    dic Graphics::get_val(stringstream &offset){
        dic pic;
        offset>>std::ws;
        getline(offset,pic.key,'=');

        for(char &lower:pic.key){
            lower=tolower(lower);
        }

        getline(offset,pic.value);
        return pic;
    }




    void Graphics::load_mann(bytes data){
        stringstream offset(string((char*)data.data(),data.size()));
        vector<string>file;
        load_mann(offset,file);
    }

    void Graphics::load_ann(bytes data){
        bytes::iterator offset=data.begin();
        load_ann(offset);
    }

    bytes Graphics::get_ann(){
        bytes data;
        back_insert_iterator<bytes> offset(data);
        get_ann(offset);
        return data;
    }

    bytes Graphics::get_mann(){
        ostringstream offset;
        vector<std::string> files;
        get_mann(offset,files);
        string data=offset.str();
        return bytes(data.begin(),data.end());
    }


    dic Graphics::load_mann(stringstream &offset,vector<string>&file){
        cout<<"You shouldn't be here"<<endl;
        dic pic;
        return pic;
    }

    void Graphics::load_ann(bytes::iterator &offset){
        cout<<"You shouldn't be here"<<endl;
    }

    void Graphics::get_mann(ostringstream &offset,vector<std::string>&files){
        cout<<offset.str()<<endl;

    }

    void Graphics::get_ann(back_insert_iterator<bytes> &offset){
        cout<<"You shouldn't be here"<<endl;
    }

    int Graphics::add_file(std::vector<std::string> &files,string file){
        for(int i=0;i<files.size();i++){
            if(files[i]==file){
                return i;
            }
        }
        files.push_back(file);
        return files.size()-1;
    }
}
