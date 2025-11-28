#include <iostream>
#include <windows.h>
#include <ctime>
#include <vector>

using namespace std;

vector<int> massive;
HANDLE startEvent;
HANDLE* blockedEvents;

DWORD WINAPI marker(LPVOID argument_number)
{
	WaitForSingleObject(startEvent, INFINITE);
	int sequence_number = (int)(intptr_t)argument_number;
	srand(sequence_number);
	int count_of_marked = 0;
	bool is_marked = true;
	while (is_marked)
	{
		int random = rand();
		int index = random % massive.size();
		if (massive[index] == 0)
		{
			Sleep(5);
			massive[index] = sequence_number;
			count_of_marked = count_of_marked + 1;
		}
		else
		{
			cout << "Sequence number: " << sequence_number << endl;
			cout << "Count of marked elements: " << count_of_marked << endl;
			cout << "Index of unmarked element: " << index << endl;
			is_marked = false;
			SetEvent(blockedEvents[sequence_number]);
			SuspendThread(GetCurrentThread);
		}
	}
	return 0;
}

int Input()
{
	cout << "Input massive size: ";
	int size;
	cin >> size;
	massive.resize(size, 0);
	cout << "Input the number of threads: ";
	int number_of_threads;
	cin >> number_of_threads;
	return number_of_threads;
}

void Create_threads(int number_of_threads)
{
	startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD* ID_markerThread = new DWORD[number_of_threads];
	HANDLE* markers = new HANDLE[number_of_threads];
	for (int i = 0; i < number_of_threads; i++)
	{
		markers[i] = CreateThread(NULL, 0, marker, (LPVOID)(intptr_t)i, 0, &ID_markerThread[i]);
	}

	blockedEvents = new HANDLE[number_of_threads];
	for (int i = 0; i < number_of_threads; i++)
	{
		blockedEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	if (startEvent != NULL)
	{
		SetEvent(startEvent);
	}
	WaitForMultipleObjects(number_of_threads, blockedEvents, TRUE, INFINITE);
}

void Massive_output()
{
	cout << "Massive: " << massive[0];
	for (int i = 0; i < massive.size(); i++)
	{
		cout << " " << massive[i];
	}
	cout << endl;
}

int Input_index()
{
	cout << "Input the number of the thread to complete";
	int complete_thread_number;
	cin >> complete_thread_number;
	return complete_thread_number;
}

int main()
{
	int number_of_threads = Input();
	Create_threads(number_of_threads);
	Massive_output();
	int complete_thread_number = Input_index();

	return 0;
}