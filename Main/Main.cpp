#include "Main.h"
using namespace std;

std::vector<int> massive;
CRITICAL_SECTION massLock;
HANDLE startEvent = NULL;

void Create_critical_sections_and_events() 
{
    InitializeCriticalSection(&massLock);
    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void Create_markers(vector<Marker*>& markers, int number_of_threads) 
{
    for (int i = 0; i < number_of_threads; ++i) 
    {
        markers.push_back(new Marker(i + 1, startEvent, &massLock, massive));
    }
}

int Find_complete_thread_index(vector<Marker*> active_markers, int complete_thread_number)
{
    int index = 0;
    while (index < active_markers.size())
    {
        if (active_markers[index]->getSequenceNumber() == complete_thread_number)
        {
            break;
        }
        ++index;
    }

    if (index == active_markers.size())
    {
        cout << endl << "There is no thread with this sequence number!" << endl;
        return - 1;
    }
    
    return index;
}

void Delete_complete_thread(vector<Marker*>& active_markers, vector<HANDLE>& active_blocked_events, int index)
{
    delete active_markers[index];
    active_markers.erase(active_markers.begin() + index);
    active_blocked_events.erase(active_blocked_events.begin() + index);
}

int main()
{
    int size = Input_size();
    massive.resize(size, 0);
    Create_critical_sections_and_events();
    int number_of_threads = Input_threads_number();
    vector<Marker*> active_markers;
    Create_markers(active_markers, number_of_threads);
    vector<HANDLE> active_blocked_events;
    for(Marker* m : active_markers)
    {
        active_blocked_events.push_back(m->getBlockedEvent());
    }

    SetEvent(startEvent);

    while (!active_markers.empty()) 
    {
        WaitForMultipleObjects(static_cast<DWORD>(active_blocked_events.size()), active_blocked_events.data(), TRUE, INFINITE);
        Massive_output();

        int complete_thread_number = Input_index();
        int index = Find_complete_thread_index(active_markers, complete_thread_number);
        if(index == -1)
        {
            return 1;
        }

        active_markers[index]->setAction(Finish);
        SetEvent(active_markers[index]->getControlEvent());
        WaitForSingleObject(active_markers[index]->getThreadHandle(), INFINITE);
        Delete_complete_thread(active_markers, active_blocked_events, index);
        Massive_output();

        for (Marker* m : active_markers)
        {
            m->setAction(Continue);
            SetEvent(m->getControlEvent());
        }
    }

    DeleteCriticalSection(&massLock);
    CloseHandle(startEvent);

    return 0;
}