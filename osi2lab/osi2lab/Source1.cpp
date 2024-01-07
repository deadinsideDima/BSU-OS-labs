#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

//void createChildProcess(const char* commandLine) {
//    STARTUPINFOW si;
//    PROCESS_INFORMATION pi;
//
//    ZeroMemory(&si, sizeof(STARTUPINFOW));
//    si.cb = sizeof(STARTUPINFOW);
//    //ZeroMemory(&pi, sizeof(pi));
//    si.dwFlags = STARTF_USESHOWWINDOW;
//    si.wShowWindow = SW_SHOWNORMAL;
//    si.cb = sizeof(STARTUPINFO);
//    si.lpTitle = (LPWSTR)L"Child";
//
//    if (!CreateProcess(NULL, (LPWSTR)commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
//        cerr << "CreateProcess failed (" << GetLastError() << ")" << endl;
//        return;
//    }
//    WaitForSingleObject(pi.hProcess, INFINITE);
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//}

int main() {
    SetConsoleTitle(L"Parent Window");

    cout << "Enter the size of the array: ";
    int n;
    cin >> n;
    cout << "Enter the elements of the array: " << endl;
    cin.ignore();
    string commandLine = "ConsoleApplication5.exe " + to_string(n);
    for (int i = 0; i < n; ++i) {
        int element;
        cout << "Element " << i + 1 << ": ";
        cin >> element;
        commandLine += " " + to_string(element);
    }
    const char* command = commandLine.c_str();
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWMINIMIZED;
    si.cb = sizeof(STARTUPINFO);
    si.lpTitle = (LPWSTR)L"Child";
    int len = strlen(command) + 1;
    int size = MultiByteToWideChar(CP_UTF8, 0, command, len, NULL, 0);
    wchar_t* wCommand = new wchar_t[size];
    MultiByteToWideChar(CP_UTF8, 0, command, len, wCommand, size);

    if (CreateProcess(NULL, (LPWSTR)wCommand, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        cerr << "Process Child was created" << endl;
    }
    else {
        std::cerr << "Wrong with creation of process Child" << std::endl;
        return 1;
    }
    //createChildProcess(commandLine.c_str());
    return 0;
}
