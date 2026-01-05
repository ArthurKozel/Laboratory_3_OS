#include "Main.h"
using namespace std;

vector<int> massive;
CRITICAL_SECTION massLock;
HANDLE startEvent = NULL;

int main()
{
    int size = Input_size();
    massive.resize(size, 0);
    Create_critical_sections_and_events(massLock, startEvent);
    int number_of_threads = Input_threads_number();
    vector<Marker*> active_markers = Create_active_markers(number_of_threads, massive, massLock, startEvent);
    vector<HANDLE> active_blocked_events = Create_active_blocked_events(active_markers);

    SetEvent(startEvent);

    while (!active_markers.empty()) 
    {
        WaitForMultipleObjects(static_cast<DWORD>(active_blocked_events.size()), active_blocked_events.data(), TRUE, INFINITE);
        Massive_output();

        int complete_thread_number = Input_index();
        int index = Find_complete_thread_index(active_markers, complete_thread_number);
        if(index == -1)
        { return 1; }

        Finish_and_clean_chosen_thread(active_markers, active_blocked_events, index);
        Massive_output();

        Continue_others_threads(active_markers);
    }

    DeleteCriticalSection(&massLock);
    CloseHandle(startEvent);

    return 0;
}