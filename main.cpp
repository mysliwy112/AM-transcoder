#include <iostream>
#include <string>
#include <vector>

#include "PNGformat.h"
#include "ANN.h"
#include "filesys.h"

using namespace std;


bool local=false;
bool logg=false;
bool to_raw=false;
bool lining=false;

bool name_dir=false;
string out_directory;

bool seqence=false;
string event_name;

void help_page(){
    cout<<"Decode >ANN<";

    #if BIGFILE
        cout<<" (BIGFILE enabled)";
    #endif // BIGFILE

    cout<<endl<<endl;
    cout<<"anndrzem [file paths][-h][-f directory_path][-l][-o][-s event_name][-e][-n]"<<endl<<endl;
    cout<<"-h\tShows help message."<<endl;
    cout<<"-f\tSets output directory."<<endl;
    cout<<"-l\tSome random console output."<<endl;
    cout<<"-o\tDon't create new directory for images."<<endl;
    cout<<"-s\tCreates event sequence (type \"id:\" to name by id)(events names are going to be listed and can be chosen from on runtime)."<<endl;
    cout<<"-e\tExtracts raw decompressed bitmap in RGBA(8,8,8,8)."<<endl;
    cout<<"-n\tAlign image sizes."<<endl;

    cout<<endl;
}

int parse_commandline(char *command[],int arg,int maxi){
    vector<string> files;
    if(command[arg][1]=='h'){
        help_page();
    }else if(command[arg][1]=='l'){
        logg=true;
        return 0;
    }else if(command[arg][1]=='f'){
        name_dir=true;
        out_directory=command[arg+1];
        return 1;
    }else if(command[arg][1]=='o'){
        local=true;
        return 0;
    }else if(command[arg][1]=='s'){
        seqence=true;
        if(arg+1>=maxi)
            return 0;
        if(command[arg+1][0]=='-'||command[arg+1][0]=='/'){
            return 0;
        }else{
            event_name=command[arg+1];
            return 1;
        }
    }else if(command[arg][1]=='e'){
        seqence=true;
        return 0;
    }else if(command[arg][1]=='n'){
        lining=true;
        return 0;
    }
    cout<<"Bad parameter, skipping..."<<endl;
    return 0;
}


int main(int argc, char *argv[])
{
    int files=0;
    int f_file=0;
    bool f_count=false;

    if(argc>1){
        try{
            for(int arg=1;arg<argc;arg++){
                if(argv[arg][0]=='-'||argv[arg][0]=='/'){
                    arg+=parse_commandline(argv,arg,argc);
                    if(files>0){
                        f_count=true;
                    }

                }else{
                    if(f_count==true){
                        throw invalid_argument("Bad commandline parameters.");
                    }
                    if(files==0){
                        f_file=arg;
                    }
                    if(f_count==false){
                        files++;
                    }
                }

            }
        }catch(invalid_argument&e){
            cout<<e.what()<<endl;
            return 0;
        }

        cout<<"Extraction in progress..."<<endl;
        for(int i=f_file;i<files+f_file;i++){
            //try{
                ANN ann;
                string filename(argv[i]);

                ann.set_log(logg);
                ann.set_raw(to_raw);
                ann.set_align(lining);
                if(seqence)
                    ann.set_sequence(seqence,event_name);

                ann.load_ann(filename);

                string dir;
                if(name_dir==false){
                    dir=get_directory(argv[0])+string("\\");
                }else{
                    dir=out_directory;
                    if(dir.back()!='\\'&&dir.back()!='/'){
                        dir+=string("\\");
                    }
                }

                if(local==false){
                    dir+=get_file_name(filename)+string("\\");
                }
                create_directory(dir);
                ann.extract_ANN(dir);
            //}catch(...){
                //cout<<"Can't process ann file, moving to the next file."<<endl;
            //}
        }
        cout<<"No more files to decode."<<endl;
    }else{
        help_page();
    }
    return 0;
}


