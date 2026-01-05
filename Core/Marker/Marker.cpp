#include "Marker.h"
using namespace std;

Marker::Marker(int sequenceNumber, HANDLE& startEvent, CRITICAL_SECTION* massLock, vector<int>& massive)
    : sequenceNumber(sequenceNumber), startEvent(startEvent), massLock(massLock), massive(massive), status(Continue)
{
    blockedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    controlEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    handle = CreateThread(NULL, 0, markerThread, this, 0, &ID_markerThread);
}

Marker::~Marker()
{
    CloseHandle(blockedEvent);
    CloseHandle(controlEvent);
    CloseHandle(handle);
}

HANDLE Marker::getBlockedEvent() const 
{
    return blockedEvent;
}

HANDLE Marker::getControlEvent() const
{
    return controlEvent;
}

HANDLE Marker::getThreadHandle() const 
{
    return handle;
}

int Marker::getSequenceNumber() const 
{
    return sequenceNumber;
}

void Marker::setAction(Action action) 
{
    status = action;
}

DWORD WINAPI Marker::markerThread(LPVOID arguments)
{
    return ((Marker*)arguments)->marker();
}

DWORD Marker::marker() 
{
    WaitForSingleObject(startEvent, INFINITE);
    srand(sequenceNumber);
    int count_of_marked = 0;
    bool is_active = true;
    while (is_active) 
    {
        int random = rand();
        int index = random % massive.size();
        EnterCriticalSection(massLock);
        if (massive[index] == 0)
        {
            LeaveCriticalSection(massLock);
            Sleep(5);
            EnterCriticalSection(massLock);
            massive[index] = sequenceNumber;
            LeaveCriticalSection(massLock);
            count_of_marked++;
            Sleep(5);
        }
        else 
        {
            cout << endl << "Sequence number: " << sequenceNumber << endl;
            cout << "Count of marked elements: " << count_of_marked << endl;
            cout << "Index of unmarked element: " << index << endl;
            LeaveCriticalSection(massLock);
            SetEvent(blockedEvent);
            WaitForSingleObject(controlEvent, INFINITE);
            if (status == Finish) 
            {
                EnterCriticalSection(massLock);
                for (int j = 0; j < massive.size(); ++j) 
                {
                    if (massive[j] == sequenceNumber)
                    {
                        massive[j] = 0;
                    }
                }
                LeaveCriticalSection(massLock);
                return 0;
            }
            else 
            {
                ResetEvent(blockedEvent);
            }
        }
    }
    return 0;
}