#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include "../Marker/Marker.h"

void Create_critical_sections_and_events(CRITICAL_SECTION& massLock, HANDLE& startEvent);

std::vector<Marker*> Create_active_markers(int number_of_threads, std::vector<int>& massive, CRITICAL_SECTION& massLock, HANDLE& startEvent);

std::vector<HANDLE> Create_active_blocked_events(std::vector<Marker*>& active_markers);

int Find_complete_thread_index(std::vector<Marker*> active_markers, int complete_thread_number);

void Delete_complete_thread(std::vector<Marker*>& active_markers, std::vector<HANDLE>& active_blocked_events, int index);

void Finish_and_clean_chosen_thread(std::vector<Marker*>& active_markers, std::vector<HANDLE>& active_blocked_events, int index);

void Continue_others_threads(std::vector<Marker*>& active_markers);