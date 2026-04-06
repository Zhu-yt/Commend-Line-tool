#include <iostream>
#include <windows.h>

int main(){
//traverse the current folder & print all file names
    using namespace std;

    WIN32_FIND_DATAA findData;

    HANDLE hFind = FindFirstFileA(".//*", &findData);
    if(hFind == INVALID_HANDLE_VALUE){
        cout << "failed to open folder" << endl;
        return 1;
    }

    do{
        cout << findData.cFileName << endl;
    }while(FindNextFileA(hFind,&findData));

    FindClose(hFind);

    return 0;
}