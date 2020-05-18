#ifndef CRLE_H
#define CRLE_H

#include <vector>

std::vector<unsigned char> decodeCRLE(std::vector<unsigned char> data,unsigned int bulk=1/*how many bytes decode as one element*/){

    std::vector<unsigned char> n;
    unsigned int i=0;
    while(i<data.size()){
        if(data[i]<128){
            n.insert(n.end(),data.begin()+i+1,data.begin()+i+data[i]*bulk+1);
            i+=data[i]*bulk+1;
        }else{
            data[i]-=128;
            int var=n.size();
            n.resize(n.size()+data[i]*bulk);
            for(unsigned int k=0;k<data[i];k++){
                for(unsigned int l=0;l<bulk;l++){
                    n[var+k*bulk+l]=data[i+l+1];
                }
            }
            i+=1+bulk;
        }
    }
    return n;
}

std::vector<unsigned char> codeCRLE(std::vector<unsigned char> data,unsigned int bulk=1/*how many bytes decode as one element*/){
    std::vector<unsigned char> n;
    return data;
}

#endif // CRLE_H
