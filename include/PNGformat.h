#ifndef PNGFORMAT_H
#define PNGFORMAT_H

#include<vector>
#include<string>

#include "Image.h"

void write_PNG(std::vector<unsigned char> &data, unsigned long long width, unsigned long long height, int bpp, std::string &filename);
am::Image read_PNG(std::string filename);
am::Image load_PNG(std::vector<unsigned char> &data);
am::Image load_JPEG(std::vector<unsigned char> &from);
#endif // PNGFORMAT_H
