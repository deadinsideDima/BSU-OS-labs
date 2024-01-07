#include <Windows.h>
#include <iostream>

using namespace std;

struct wStruct {
	unsigned int* mas;
	size_t size;
};

DWORD WINAPI worker(LPVOID lpParameters) {
	auto* p = (wStruct*)lpParameters;
	for (int i = 0; i < p->size; i++) {
		if (p->mas[i] % 10 == 1) {
			cout << p->mas[i] << " ";
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI count(LPVOID mas) {
	int i = 0;
	long long a = 0;
	long long b = 1;
	while (true) {
		std::cout << a << " ";
		long long temp = a;
		a = b;
		b += temp;
		Sleep(50);
		i++;
	}
	return 0;
}


int main()
{
	wStruct a;
	cout << "Enter the size of array:" << endl;
	cin >> a.size;
	a.mas = new unsigned int[a.size];
	cout << "Enter the elements:" << endl;
	for (int i = 0; i < a.size; i++) {
		cin >> a.mas[i];
	}
	wStruct b;
	b.size = a.size;
	b.mas = new unsigned int[b.size];
	for (int i = 0; i < a.size; i++) {
		b.mas[i] = 0;
	}
	HANDLE thread;
	DWORD idThread;
	thread = CreateThread(NULL, 0, worker, (void*)(&a), 0, &idThread);
	if (thread == NULL) {
		return GetLastError();
	}
	cout << "Thread worker is started!" << endl;
	DWORD idCount;
	HANDLE hCount = CreateThread(NULL, 0, count, (void*)(&b), CREATE_SUSPENDED, &idCount);
	if (hCount == NULL) {
		return GetLastError();
	}
	Sleep(10);
	SuspendThread(thread);
	Sleep(10);
	ResumeThread(thread);
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	cout << endl << "Thread worker end working!" << endl;
	ResumeThread(hCount);
	cout << "Counter start working!" << endl;
	WaitForSingleObject(hCount, 1000);
	Sleep(10);
	CloseHandle(hCount);
	cout << endl << "Counter stop working!" << endl;
	return 0;
}