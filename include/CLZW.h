#ifndef CLZW_H
#define CLZW_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "piklib8_shim.h"

std::vector<unsigned char> decodeCLZW(std::vector<unsigned char> &code){

    long long out_size=code[0]+code[1]*0x100+code[2]*0x10000+code[3]*0x1000000;
    char *output;
    output = piklib_CLZWCompression2_decompress(reinterpret_cast<char*>(code.data()), code.size());
    std::vector<unsigned char> data(out_size);
    std::copy(reinterpret_cast<char*>(output), reinterpret_cast<char*>(output)+out_size, data.begin());

    delete[] output;
    return data;
}

std::vector<unsigned char> codeCLZW(std::vector<unsigned char> &decode){

    long long in_size=decode.size();

    int out_size;
    char *output;
    output = piklib_CLZWCompression2_compress(reinterpret_cast<char*>(decode.data()), decode.size(),&out_size);
    std::vector<unsigned char> data(out_size+8);
    std::copy(reinterpret_cast<char*>(output), reinterpret_cast<char*>(output)+out_size+8, data.begin());

    delete[] output;
    return data;
}

#endif // CLZW_H
