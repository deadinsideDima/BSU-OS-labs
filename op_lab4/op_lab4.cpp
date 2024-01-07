#include <iostream>
#include <windows.h>
#include <string>



int main()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, "WriterMutex");
    HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, "ReaderSemaphore");
    if (hMutex == NULL || hSemaphore == NULL) {
        return GetLastError();
    }
    HANDLE writerEvents[5];
    HANDLE readerEvents[3];
    for (int i = 0; i < 3; ++i) {
        writerEvents[i] = CreateEvent(NULL, FALSE, FALSE, ("WriterEvent" + std::to_string(i)).c_str());
        readerEvents[i] = CreateEvent(NULL, FALSE, FALSE, ("ReaderEvent" + std::to_string(i)).c_str());
        if (writerEvents[i] == NULL || readerEvents[i] == NULL) {
            return GetLastError();
        }
    }
    writerEvents[3] = CreateEvent(NULL, FALSE, FALSE, "WriterEndEvent");
    writerEvents[4] = CreateEvent(NULL, FALSE, FALSE, "ReaderEndEvent");
    int processNum, messageNum;
    std::cout << "Enter number of Writer(Reader) processes: ";
    std::cin >> processNum;
    std::cout << "Enter number of messages: ";
    std::cin >> messageNum;
    STARTUPINFO si;
    PROCESS_INFORMATION* writer_pi = new PROCESS_INFORMATION[processNum];
    PROCESS_INFORMATION* reader_pi = new PROCESS_INFORMATION[processNum];
    std::string writerInfo = std::to_string(messageNum);
    std::string readerInfo;

    for (int i = 0; i < processNum; ++i) {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess("Debug\\Writer.exe", (char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &writer_pi[i])) {
            std::cout << "\nwriter process is not created\n";
            return GetLastError();
        }
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess("Debug\\Reader.exe", (char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &reader_pi[i])) {
            std::cout << "\nreader process is not created\n";
            return GetLastError();
        }
    }

    int writerNum = processNum;
    int readerNum = processNum;
    while (writerNum || readerNum) {
        DWORD message = WaitForMultipleObjects(5, writerEvents, FALSE, INFINITE);
        switch (message) {
            case 0:
                std::cout << "Recieved message: A\n";
                PulseEvent(readerEvents[0]);
                break;
            case 1:
                std::cout << "Recieved message: B\n";
                PulseEvent(readerEvents[1]);
                break;
            case 2:
                std::cout << "Recieved message: C\n";
                PulseEvent(readerEvents[2]);
                break;
            case 3:
                std::cout << "Writer Process ended\n";
                --writerNum;
                break;
            case 4:
                std::cout << "Reader Process ended\n";
                --readerNum;
                break;
            
        }
    }
    

    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    for (int i = 0; i < 3; ++i) {
        CloseHandle(writerEvents[i]);
        CloseHandle(readerEvents[i]);
    }
    CloseHandle(writerEvents[3]);
    CloseHandle(writerEvents[4]);
    for (int i = 0; i < processNum; ++i) {
        CloseHandle(writer_pi[i].hThread);
        CloseHandle(writer_pi[i].hProcess);
        CloseHandle(reader_pi[i].hThread);
        CloseHandle(reader_pi[i].hProcess);
    }
    return 0;
}
