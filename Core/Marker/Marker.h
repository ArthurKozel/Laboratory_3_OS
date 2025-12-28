#pragma once
#include <iostream>
#include <windows.h>
#include <ctime>
#include <vector>

enum Action {Continue, Finish};

class Marker
{
private:
	HANDLE handle;
	Action status;
	int sequenceNumber;
	HANDLE blockedEvent;
	HANDLE controlEvent;
	CRITICAL_SECTION* massLock;
	HANDLE& startEvent;
	std::vector<int>& massive;
	DWORD ID_markerThread;
public:

	Marker(int sequence_number, HANDLE& startEvent, CRITICAL_SECTION* massLock, std::vector<int>& massive);
	~Marker();


	HANDLE getBlockedEvent() const;
	HANDLE getControlEvent() const;
	HANDLE getThreadHandle() const;
	int getSequenceNumber() const;
	void setAction(Action a);


	static DWORD WINAPI markerThread(LPVOID arguments);
	DWORD marker();
};