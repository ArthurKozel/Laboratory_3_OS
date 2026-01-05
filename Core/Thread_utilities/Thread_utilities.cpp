#include "Thread_utilities.h"

using namespace std;


void Create_critical_sections_and_events(CRITICAL_SECTION& massLock, HANDLE& startEvent)
{
    InitializeCriticalSection(&massLock);
    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

vector<Marker*> Create_active_markers(int number_of_threads, vector<int>& massive, CRITICAL_SECTION& massLock, HANDLE& startEvent)
{
    vector<Marker*> active_markers;
    for (int i = 0; i < number_of_threads; ++i)
    {
        active_markers.push_back(new Marker(i + 1, startEvent, &massLock, massive));
    }

    return active_markers;
}

vector<HANDLE> Create_active_blocked_events(vector<Marker*>& active_markers)
{
    vector<HANDLE> active_blocked_events;
    for (Marker* m : active_markers)
    {
        active_blocked_events.push_back(m->getBlockedEvent());
    }

    return active_blocked_events;
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
        return -1;
    }

    return index;
}

void Delete_complete_thread(vector<Marker*>& active_markers, vector<HANDLE>& active_blocked_events, int index)
{
    delete active_markers[index];
    active_markers.erase(active_markers.begin() + index);
    active_blocked_events.erase(active_blocked_events.begin() + index);
}

void Finish_and_clean_chosen_thread(vector<Marker*>& active_markers, vector<HANDLE>& active_blocked_events, int index)
{
    active_markers[index]->setAction(Finish);
    SetEvent(active_markers[index]->getControlEvent());
    WaitForSingleObject(active_markers[index]->getThreadHandle(), INFINITE);
    Delete_complete_thread(active_markers, active_blocked_events, index);
}

void Continue_others_threads(vector<Marker*>& active_markers)
{
    for (Marker* m : active_markers)
    {
        m->setAction(Continue);
        SetEvent(m->getControlEvent());
    }
}