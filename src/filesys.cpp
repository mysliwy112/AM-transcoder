#include "filesys.h"
#include "am_utils.h"

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void create_directory(string dir){
    int last=0;
    while (last!=string::npos){
        last=dir.find_first_of("\\/",last+1);
        string creator=dir.substr(0,last);
        CreateDirectoryA(creator.c_str(),NULL);
    }
}

void delete_file(string fil){
    DeleteFileA(fil.c_str());
}

string get_directory(string filename){
    if(filename.find_last_of("\\/")==string::npos)
        return "./";
    else
        return filename.substr(0,filename.find_last_of("\\/"))+"/";
}

string get_file_name(string filename){


    if(filename.find_last_of("\\/")==string::npos)
        return filename.substr(0,filename.find_last_of('.'));
    else if(filename.find_last_of("\\/")<filename.find_last_of('.'))
        return filename.substr(filename.find_last_of("\\/")+1,filename.find_last_of('.')-(filename.find_last_of("\\/"))-1);
    else
        return filename.substr(filename.find_last_of("\\/")+1);
}

vector<unsigned char> read_file(string filename, unsigned long long length){

    ifstream file(filename,ios::binary);
    vector<unsigned char> data;

    if(!file.good()){
        throw invalid_argument(string("Can't access file: ")+filename);
    }

    file.seekg (0, ios::end);

    unsigned long long f_size=file.tellg();
    file.seekg (0, ios::beg);
    if(f_size>length&&length!=0)
        f_size=length;
    if(f_size>=data.max_size()){
        file.close();
        throw length_error("File is too big.");
    }

    data.resize(f_size);

    file.read((char*)(&data[0]),f_size);
    file.close();

    return data;
}

void write_file(std::string filename, std::vector<unsigned char> data){
    int i=2;
    while(i<filename.size()){
        i=filename.find_first_of("<>:\"|?*",i);
        if(i==string::npos){
            break;
        }
        filename.replace(i,1,"+");
        i++;
    }

    ofstream file(filename,ios::binary);
    if(!file.good()){
        file.close();
        throw"Can't save file: "+filename;
    }

    file.write((char*)data.data(),data.size());
    file.close();
}

int get_pad_len(int size){
    int pad_len;
    if(am::PAD>=len_int(size))
        pad_len=am::PAD;
    else
        pad_len=len_int(size);
    return pad_len;
}

string pad_int(unsigned int number,int length){
    if (number == 0)
        return string(length,'0');

    int digits=len_int(number);

    return string(length-digits,'0')+to_string(number);
}

unsigned int len_int(unsigned int number){
    unsigned int digits=0;

    while (number) {
        number /= 10;
        digits++;
    }

    return digits;
}
