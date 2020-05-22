#ifndef CRLE_H
#define CRLE_H

#include <vector>
using namespace std;
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


    if(float(data.size())/bulk>float(int(data.size()/bulk))){
        std::cout<<"Not matching data size.\n";
        throw"Incompatible data size.";
    }


    std::vector<unsigned char> n;

    unsigned char counter=1;
    int point=bulk;
    int last=0;

    while(point<data.size()){
        if(counter<126&&std::equal(data.begin()+point-bulk,data.begin()+point,data.begin()+point)){
            counter++;
        }else{
            if(counter>=3){
                if((point-last)/bulk>counter){
                    n.push_back((point-last)/bulk-counter);
                    n.insert(n.end(),data.begin()+last,data.begin()+point-counter*bulk);
                }
                last=point;
                n.push_back(counter+128);
                n.insert(n.end(),data.begin()+point-bulk,data.begin()+point);
            }else if(point-last>=127*bulk){
                n.push_back(127);
                n.insert(n.end(),data.begin()+last,data.begin()+last+127*bulk);
                last+=127*bulk;
            }
            counter=1;
        }
        point+=bulk;
    }
    if((point-last)/bulk>counter){
        n.push_back((point-last)/bulk-counter);
        n.insert(n.end(),data.begin()+last,data.begin()+point-counter*bulk);
    }
    last=point;
    n.push_back(counter+128);
    n.insert(n.end(),data.begin()+point-bulk,data.begin()+point);

//    std::vector<unsigned char> ne=decodeCRLE(n,bulk);
//    for(int i=0;i<data.size();i++){
//        if(data[i]!=ne[i]){
//            cout<<i<<" "<<data.size()<<endl;
//            cout<<(int)data[i]<<" "<<(int)ne[i]<<endl;
//            cout<<(int)n[i]<<" "<<(int)n[i+1]<<endl;
//            throw"";
//        }
//    }

    return n;
}

#endif // CRLE_H
