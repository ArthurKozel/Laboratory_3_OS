#include "IO_utilities.h"

using namespace std;

int Input_size()
{
	cout << "Input massive size: ";
	int size;
	cin >> size;
	return size;
}

int Input_threads_number()
{
	cout << "Input the number of threads: ";
	int number_of_threads;
	cin >> number_of_threads;
	return number_of_threads;
}

int Input_index()
{
	cout << "Input the number of the thread to complete:" << endl;
	int complete_thread_number;
	cin >> complete_thread_number;
	return complete_thread_number;
}

void Massive_output()
{
	cout << endl;

	if (massive.empty())
	{
		cout << "Massive: empty" << endl;
		return;
	}

	cout << "Massive: " << massive[0];
	for (int i = 1; i < massive.size(); i++)
	{
		cout << " " << massive[i];
	}
	cout << endl;
}
