#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

void showHelp(){
    cout << "========================================" << endl;
    cout << "   batch file renaming tool" << endl;
    cout << "========================================" << endl;
    cout << "usage:" << endl;
    cout << "  --prefix <text>    add prefix" << endl;
    cout << "  --suffix <text>    add suffix" << endl;
    cout << "  --dry-run          preview mode" << endl;
    cout << "  --help, -h         show help" << endl;
    cout << endl;
    cout << "examples:" << endl;
    cout << "  ./renamer.exe --prefix backup_           # add prefix" << endl;
    cout << "  ./renamer.exe --suffix _backup           # add suffix" << endl;
    cout << "  ./renamer.exe --prefix backup_ --dry-run # preview mode" << endl;
    cout << "========================================" << endl;
}

string addPrefix(const string& filename, const string& prefix){
    return prefix + filename;
}

string addSuffix(const string& filename, const string& suffix){
    size_t dotPos = filename.find_last_of('.');

    if(dotPos != string::npos){
        string namepart = filename.substr(0,dotPos);
        string extpart = filename.substr(dotPos);
        return namepart + suffix + extpart;
    }

    return filename + suffix;
}

void processFiles(const string& operation, const string& text, bool dryRun){
    WIN32_FIND_DATAA findData;

    //typedef void* HANDLE
    HANDLE hFind = FindFirstFileA(".//*", &findData);
    if(hFind == INVALID_HANDLE_VALUE){
        cout << "failed to open folder" << endl;
        return;
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

        string new_name;

        if(operation == "prefix"){
            new_name = addPrefix(filename, text);
        }
        if(operation == "suffix"){
            new_name = addSuffix(filename, text);
        }

        string old_path = ".\\" + filename;
        string new_path = ".\\" + new_name;

        if(dryRun){
            //dru run
            cout << "[preview] " << filename << "  ->  " << new_name << endl;
        }else{
            //rename
            if(rename(old_path.c_str(), new_path.c_str()) == 0){
                cout << "new_name:  " << new_name << endl;
            }else{
                cout << "operation failed" << endl;
            }
            
        }
        
    }while(FindNextFileA(hFind,&findData));

    FindClose(hFind);
}

int main(int argc, char* argv[]){//argc: number of argument; argv: argument content
    
    if(argc < 3){
        showHelp();
        return 1;
    }

    string operation;
    string text;
    bool dryRun = false;

    for(int i = 1;i < argc;i++){
        string arg = argv[i];
        
        if(arg == "--prefix"){
            operation = "prefix";

            if(i + 1 < argc){
                text = argv[++i];
            }

        }else if(arg == "--suffix"){
            operation = "suffix";

            if(i + 1 < argc){
                text = argv[++i];
            }

        }else if(arg == "--dry-run"){
            dryRun = true;

        }else if(arg == "--help" || arg == "-h"){
            showHelp();
            return 0;
        }
    }

    if(operation.empty() || text.empty()){
        cout << "Error: please specify an operation and its text" << endl;
        showHelp();
        return 1;
    }

    processFiles(operation, text, dryRun);

    return 0;
}