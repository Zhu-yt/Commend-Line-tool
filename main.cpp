#include <iostream>
#include <windows.h>

int main(){
//traverse the current folder & print all file names
    using namespace std;

    WIN32_FIND_DATAA findData;

//typedef void* HANDLE
    HANDLE hFind = FindFirstFileA(".//*", &findData);
    if(hFind == INVALID_HANDLE_VALUE){
        cout << "failed to open folder" << endl;
        return 1;
    }

//on the basis of filtering out '.' & "..", add folder judgment
    do{
        if(cFileName == '.' || cFileName == ".."){
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
        if(!(findData.dwFileAttrbutes & FILE_ATTRIBUTE_DIRECTORY)){
            continue;
        }

        cout << findData.cFileName << endl;
    }while(FindNextFileA(hFind,&findData));

    FindClose(hFind);

    return 0;
}