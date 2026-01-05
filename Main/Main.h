#pragma once
#include <iostream>
#include <windows.h>
#include <ctime>
#include <vector>
#include "../Core/IO_utilities/IO_utilities.h"
#include "../Core/Thread_utilities/Thread_utilities.h"
#include "../Core/Marker/Marker.h"


extern std::vector<int> massive;
extern HANDLE startEvent;
extern CRITICAL_SECTION massLock;