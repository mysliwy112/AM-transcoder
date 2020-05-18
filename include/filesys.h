#ifndef FILESYS_H
#define FILESYS_H

#include <string>
#include <vector>

void create_directory(std::string dir);
void delete_file(std::string fil);
std::string get_directory(std::string filename);
std::string get_file_name(std::string filename);
std::vector<unsigned char> read_file(std::string filename, unsigned long long length=0);
void write_file(std::string filename, std::vector<unsigned char> data);
std::string pad_int(unsigned int number,int length);
unsigned int len_int(unsigned int number);


#endif // FILESYS_H
