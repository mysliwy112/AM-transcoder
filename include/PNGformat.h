#ifndef PNGFORMAT_H
#define PNGFORMAT_H

#include<vector>
#include<string>

//#include "Image.h"

void write_PNG(std::vector<unsigned char> &data, unsigned long long width, unsigned long long height, int bpp, std::string &filename);
void read_PNG(std::string filename);
#endif // PNGFORMAT_H
