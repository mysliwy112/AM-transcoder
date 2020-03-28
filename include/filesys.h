#ifndef FILESYS_H
#define FILESYS_H

#include <string>

void create_directory(std::string dir);
void delete_file(std::string fil);
std::string get_directory(std::string filename);
std::string get_file_name(std::string filename);


#endif // FILESYS_H
