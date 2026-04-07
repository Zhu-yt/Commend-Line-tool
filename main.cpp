#include <iostream>
#include <windows.h>
#include <string>

int main(int argc, char* argv[]){//argc: number of argument; argv: argument content
    using namespace std;
    
//add a prefix to rename
    if(argc < 2){
        cout << "usage: ./renamer.exe <prefix>" << endl;
        return 1;
    }
    string prefix = argv[1];

    WIN32_FIND_DATAA findData;

    //typedef void* HANDLE
    HANDLE hFind = FindFirstFileA(".//*", &findData);
    if(hFind == INVALID_HANDLE_VALUE){
        cout << "failed to open folder" << endl;
        return 1;
    }

    //on the basis of filtering out "." & "..", add folder judgment
    do{
        string filename = findData.cFileName;

        if(filename == "." || filename == ".."){
            continue;
        }

        //use bitwise AND to determine whether it has the folder attribute
        /*
        In Windows, an integer is used to store multiple attributes, with each bit representing a specific attribute:       
        Bit	Value	Attribute           Chinese
        1	1	    Read-only           只读
        2	2	    Hidden              隐藏
        3	4	    System              系统
        4	8	    Volume Label        卷标
        5	16	    Directory (Folder)  文件夹
        6	32	    Archive             存档
        */
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            continue;
        }

        if(filename == "renamer.exe"){
            continue;
        }

        string new_name = prefix + filename;

        string old_path = ".\\" + filename;
        string new_path = ".\\" + new_name;
        rename(old_path.c_str(), new_path.c_str());

        cout << filename << "  ->  " << new_name << endl;
        
    }while(FindNextFileA(hFind,&findData));

    FindClose(hFind);

    return 0;
}