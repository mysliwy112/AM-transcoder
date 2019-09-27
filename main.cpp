#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include<PNGformat.h>
#include<ANN.h>
#include<filesys.h>

using namespace std;


string out_directory;
bool name_dir=true;
bool local=false;
bool logg=false;

void help_page(){
    cout<<"Dekoduje >ANN<"<<endl<<endl;
    cout<<"anndrzem [file paths][-h][-f directory path][-l][-o]"<<endl<<endl;
    cout<<"-h\tShows help message."<<endl;
    cout<<"-f\tSet output directory."<<endl;
    cout<<"-l\tSome random console output."<<endl;
    cout<<"-o\tDon't create new directory for images."<<endl;
}

void parse_commandline(char command[2]){
    vector<string> files;
            if(command[1]=='h'){
                help_page();
            }else if(command[1]=='l'){
                logg=true;
            }else if(command[1]=='f'){
                name_dir=false;
            }else if(command[1]=='o'){
                local=true;
            }

}


int main(int argc, char *argv[])
{
    int files=0;
    bool f_count=0;


    if(argc>1){
        for(int arg=1;arg<argc;arg++){
            if(argv[arg][0]=='-'||argv[arg][0]=='/'){
                parse_commandline(argv[arg]);
                f_count=true;
            }else{
                if(f_count==false)
                    files++;
                else if(name_dir==false&&out_directory.size()==0)
                    out_directory=argv[arg];
            }
        }

        for(int i=1;i<=files;i++){
            ANN ann;
            string filename(argv[i]);
            //try{
                ann.log=logg;
                ann.load_ann(filename);

                string dir;
                if(name_dir==true){
                    dir=get_directory(argv[0]);
                }else{
                    dir=out_directory;
                }
                if(local==false){
                    dir+=get_file_name(filename)+string("\\");

                }
                create_directory(dir);
                ann.extract_ANN(dir);
            //}catch(...){

           // }
        }

        delete_file("~send.send");
        delete_file("~send.send.dek");
    }else{
        help_page();
    }
    return 0;
}


