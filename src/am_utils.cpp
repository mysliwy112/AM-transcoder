#include "am_utils.h"
#include <iostream>
#include <sstream>
#include <bitset>

using namespace std;

namespace am{
    Graphics::Graphics(){}

    unsigned long long Graphics::get_int(bytes::iterator &offset,int length){

        unsigned long long var =combine(offset,length);
        advance(offset,length);
        return var;
    }


    void Graphics::set_int(bytes::iterator &offset,unsigned long long number,int length){
        bytes data(length);
        for (int i = 0; i < length; i++){
            *offset = (number >> (i * 8));
            offset++;
        }
    }

    string Graphics::get_str(bytes::iterator &offset,int length){
        string str(offset,offset+length);
        string out(str.c_str());
        advance(offset,length);
        return out;
    }

    void Graphics::set_str(bytes::iterator &offset, std::string str, int length){
        if(length==0){
            length=str.size();
        }
        for(int i=0;i<length;i++){
            *offset=str[i];
            offset++;
        }

    }

    bytes Graphics::get_data(bytes::iterator &offset,int length){
        bytes data(offset,offset+length);
        advance(offset,length);
        return data;
    }

    void Graphics::set_data(bytes::iterator &offset,bytes data){
        for(int i=0;i<data.size();i++){
            *offset=data[i];
            offset++;
        }

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
        while(pic.key.size()==0){
            getline(offset,pic.key,'=');
        }
        for(char&lower:pic.key){
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

    dic Graphics::load_mann(stringstream &offset,vector<string>&file){
        cout<<"You shouldn't be here"<<endl;
        dic pic;
        return pic;
    }

    void Graphics::load_ann(bytes::iterator &offset){
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
