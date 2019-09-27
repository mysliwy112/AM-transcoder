#ifndef CLZW_H
#define CLZW_H

#include<vector>
#include<fstream>
#include<windows.h>

using namespace std;

void startup(char* a)
{
    //LPCTSTR lpApplicationName=a;
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( NULL,   // the path
    a,        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles.
    WaitForSingleObject(pi.hProcess,INFINITE);
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

vector<unsigned char> decodeCLZW(vector<unsigned char> code, string filename="~send.send"){

    fstream file(filename,ios::out|ios::binary|ios::trunc);

    file.write((char*)code.data(),code.size());
    file.close();

    string arg="in_out_CLZW2.exe /d ";
    arg+=filename;

    startup(const_cast<char*>(arg.c_str()));

    file.open(filename+".dek",ios::in|ios::binary);

    file.seekg (0, ios::end);
    int f_size=file.tellg();
    file.seekg (0, ios::beg);
    char buffer[f_size];
    file.read(buffer,f_size);
    file.close();



    vector<unsigned char>ala (buffer,f_size+buffer);
    return vector<unsigned char>(buffer,f_size+buffer);
}



#endif // CLZW_H
