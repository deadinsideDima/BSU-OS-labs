#include <iostream>
#include <vector>
#include <thread>
#include <Windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

CRITICAL_SECTION cs_sync;
HANDLE event_work_started;
HANDLE event_mult_element_started;
double mult_result = 1.0;

void WorkThrArray(vector<double>& array, double A) {
    //int j = array.size() - 1;
    //for (size_t i = 0; i < array.size(); ++i) {
    //    if (array[i] >= A) {
    //        //this_thread::sleep_for(chrono::milliseconds(sleepInterval));
    //    }
    //    else {
    //        swap(array[i], array[j]);
    //        j--;
    //        //this_thread::sleep_for(chrono::milliseconds(sleepInterval));
    //    }
    //}
    cout << "Work Thread: ";
    for (double num : array) {
        cout << num << " ";
    }
    cout << endl;
}

void WorkThread(vector<double>& array, double A) {


    //vector<double> result(array.size(), 0);

    int sleepInterval;
    cout << "Enter sleep interval in milliseconds: ";
    cin >> sleepInterval;
    int j = array.size() - 1;
    int i = 0;
    while (i <= j) {
        while (i <= j and array[i] < A) {
            i++;
            this_thread::sleep_for(chrono::milliseconds(sleepInterval));
        }

        while (i <= j and array[j] >= A) {
            j--;
            this_thread::sleep_for(chrono::milliseconds(sleepInterval));
        }

        if (i <= j) {
            swap(array[i], array[j]);
            i++;
            j--;
            this_thread::sleep_for(chrono::milliseconds(sleepInterval));
        }
    }

    cout << "Work Thread: ";
    for (double num : array) {
        cout << num << " ";
    }
    cout << endl;

    SetEvent(event_mult_element_started);
    SetEvent(event_work_started);
}

void MultElementThread(const vector<double>& array, int k) {
    EnterCriticalSection(&cs_sync);
    WaitForSingleObject(event_mult_element_started, INFINITE);
    
    this_thread::sleep_for(chrono::milliseconds(100));
    for (size_t i = k; i < array.size(); ++i) {
        mult_result *= array[i];
    }

    //cout << "MultElement Result: " << mult_result << endl;

    LeaveCriticalSection(&cs_sync);
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(static_cast<unsigned int>(time(0)));

    vector<double> array;
    int k;
    double A;

    cout << "Enter the size of array: ";
    size_t size;
    cin >> size;

    for (size_t i = 0; i < size; ++i) {
        double num = rand() % 1000 / 100.0;
        array.push_back(num);
    }

    cout << "Source array: ";
    for (double num : array) {
        cout << num << " ";
    }
    cout << endl;

    cout << "Enter the values of k and A: ";
    cin >> k >> A;

    InitializeCriticalSection(&cs_sync);

    event_work_started = CreateEvent(NULL, FALSE, FALSE, NULL);
    event_mult_element_started = CreateEvent(NULL, FALSE, FALSE, NULL);

    thread work_thread(WorkThread, ref(array), A);
    thread mult_element_thread(MultElementThread, cref(array), k);
    WaitForSingleObject(event_work_started, INFINITE);
    WorkThrArray(array, A);
    EnterCriticalSection(&cs_sync);
    cout << "MultElement Result: " << mult_result << endl;
    LeaveCriticalSection(&cs_sync);
    work_thread.join();
    mult_element_thread.join();
    CloseHandle(event_work_started);
    CloseHandle(event_mult_element_started);

    DeleteCriticalSection(&cs_sync);

    return 0;
}
