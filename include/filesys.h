#ifndef FILESYS_H
#define FILESYS_H

#include <windows.h>
#include <stdexcept>
#include <string>

using namespace std;

void create_directory(string dir){
    CreateDirectoryA(dir.c_str(),NULL);
}

void delete_file(string fil){
    DeleteFileA(fil.c_str());
}

string get_directory(string filename){
    if(filename.find_last_of("\\/")==string::npos)
        return ".\\";
    else
        return filename.substr(0,filename.find_last_of("\\/"));
}

string get_file_name(string filename){
    if(filename.find_last_of("\\/")==string::npos)
        return filename.substr(0,filename.find_last_of('.'));
    else if(filename.find_last_of("\\/")<filename.find_last_of('.'))
        return filename.substr(filename.find_last_of("\\/"),filename.find_last_of('.')-(filename.find_last_of("\\/")));
    else
        return filename.substr(filename.find_last_of("\\/"));
}


#endif // FILESYS_H
