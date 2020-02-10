#ifndef CLZW_H
#define CLZW_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "piklib8_shim.h"

using namespace std;

vector<unsigned char> decodeCLZW(vector<unsigned char> code){
    long long out_size=code[0]+code[1]*0x100+code[2]*0x10000+code[3]*0x1000000;
    char *output;
    output = piklib_CLZWCompression2_decompress(reinterpret_cast<char*>(code.data()), code.size());
    vector<unsigned char> data(out_size);
    copy(reinterpret_cast<char*>(output), reinterpret_cast<char*>(output)+out_size, data.begin());

    delete[] output;
    return data;
}



#endif // CLZW_H
