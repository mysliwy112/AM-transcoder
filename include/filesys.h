#ifndef FILESYS_H
#define FILESYS_H

#include <string>

using namespace std;

void create_directory(string dir);
void delete_file(string fil);
string get_directory(string filename);
string get_file_name(string filename);


#endif // FILESYS_H
