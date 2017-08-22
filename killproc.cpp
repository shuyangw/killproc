#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <fstream>
 
#define MAX_BUFFER 256

using namespace std;

void end_proc(string proc_name_temp){
    char proc_name_char[MAX_BUFFER];
    strncpy(proc_name_char, proc_name_temp.c_str(), sizeof(proc_name_char));
    proc_name_char[sizeof(proc_name_char) - 1] = 0;

    HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
 
    PROCESSENTRY32 ProcessEntry = {0};
    ProcessEntry.dwSize = sizeof(ProcessEntry);
 
    BOOL Return = FALSE;
    Label:Return = Process32First(hProcessSnapShot,&ProcessEntry);
    
    if(!Return){
        goto Label;
    }
 
    do{
        int value = _tcsicmp(ProcessEntry.szExeFile, _T(proc_name_char)); 
        if (value == 0){
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID);
            TerminateProcess(hProcess,0);
            CloseHandle(hProcess);
        }
    
    } while(Process32Next(hProcessSnapShot, &ProcessEntry));
 
    CloseHandle(hProcessSnapShot);
}

int main() {    
    string line;
    ifstream file ("killproc.txt");
    if(file.is_open()){
        while(getline(file, line)){
            string msg = "Killing " + line + "...\n";
            cout << msg;
            end_proc(line);
        }
        file.close();
        cout << "\nFinished!\n";
    }
    else{
        cout << "File does not exist\n";
    }
}