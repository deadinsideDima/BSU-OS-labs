#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

int main() {
    int sizeOfArray;
    cout << "Input size of array: ";
    cin >> sizeOfArray;

    vector<float> array(sizeOfArray);

    int choice;
    cout << "Choose input method (1 for manual input, 2 for random generation): ";
    cin >> choice;

    if (choice == 1) {
        cout << "Input elements of the array: " << endl;
        for (int i = 0; i < sizeOfArray; ++i) {
            cout << "Input element " << i + 1 << ": ";
            cin >> array[i];
        }
    }
    else if (choice == 2) {
        srand(static_cast<unsigned int>(time(nullptr)));
        cout << "Generated array: ";
        for (int i = 0; i < sizeOfArray; ++i) {
            array[i] = rand() % 100;
            cout << array[i] << " ";
        }
        cout << endl;
    }
    else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    HANDLE hWritePipe, hReadPipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    STARTUPINFO stp;
    PROCESS_INFORMATION pi;
    ZeroMemory(&stp, sizeof(STARTUPINFO));
    stp.cb = sizeof(STARTUPINFO);
    wchar_t commandLine[80];
    wsprintf(commandLine, L"Simple.exe %d %d", (int)hWritePipe, (int)hReadPipe);
    CreateProcess(NULL, commandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &stp, &pi);

    DWORD dwBytesWritten;
    WriteFile(hWritePipe, &sizeOfArray, sizeof(int), &dwBytesWritten, NULL);
    WriteFile(hWritePipe, &array[0], sizeof(float) * sizeOfArray, &dwBytesWritten, NULL);

    CloseHandle(hWritePipe);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwBytesRead;
    int sizeOfNewArray;
    ReadFile(hReadPipe, &sizeOfNewArray, sizeof(int), &dwBytesRead, NULL);
    vector<float> resultArray(sizeOfNewArray);

    cout << "Received array from Union Process: ";
    for (int i = 0; i < sizeOfNewArray; ++i) {
        ReadFile(hReadPipe, &resultArray[i], sizeof(float), &dwBytesRead, NULL);
        cout << resultArray[i] << " ";
    }
    cout << endl;

    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
