#ifndef CRLE_H
#define CRLE_H

#include<vector>

using namespace std;

vector<unsigned char> decodeCRLE(vector<unsigned char> data,int bulk=1){
    vector<unsigned char> n;
    int i=0;
    while(i<data.size()){
        if(data[i]<128){
            n.insert(n.end(),data.begin()+i+1,data.begin()+i+data[i]*bulk+1);
            i+=data[i]*bulk+1;
        }else{
            data[i]-=128;
            int var=n.size();
            n.resize(n.size()+data[i]*bulk);
            for(int k=0;k<data[i];k++){
                for(int l=0;l<bulk;l++){
                    n[var+k*bulk+l]=data[i+l+1];
                }
            }
            i+=1+bulk;
        }
    }
    return n;
}

#endif CRLE_H
