#pragma warning(push)
#pragma warning(disable: 26495 6001 6387 6031)

#define BOOST_TEST_MODULE MyTests
#include <boost/test/included/unit_test.hpp>
#include "../Core/Thread_utilities/Thread_utilities.h"

using namespace std;

BOOST_AUTO_TEST_CASE(Test_Create_critical_sections_and_events)
{
    CRITICAL_SECTION massLock;
    HANDLE startEvent = NULL;

    Create_critical_sections_and_events(massLock, startEvent);
    HANDLE expect = NULL;
    BOOST_TEST(startEvent != expect);
}

BOOST_AUTO_TEST_CASE(Test_Create_active_markers)
{
    int number_of_threads = 2;
    vector<int> massive(7, 0);
    CRITICAL_SECTION massLock;
    HANDLE startEvent = NULL;

    Create_critical_sections_and_events(massLock, startEvent);


    vector<Marker*> mas = Create_active_markers(number_of_threads, massive, massLock, startEvent);

    Marker obj_1(1, startEvent, &massLock, massive);
    Marker obj_2(2, startEvent, &massLock, massive);
    Marker* obj_1_ptr = &obj_1;
    Marker* obj_2_ptr = &obj_2;
    vector<Marker*> expect { obj_1_ptr, obj_2_ptr };

    BOOST_REQUIRE_EQUAL(mas.size(), expect.size());
    for (int i = 0; i < mas.size(); ++i)
    {
        BOOST_TEST(mas[i]->getSequenceNumber() == expect[i]->getSequenceNumber());
    }
}

BOOST_AUTO_TEST_CASE(Test_Find_complete_thread_index)
{
    vector<int> massive(4, 0);
    CRITICAL_SECTION massLock;
    HANDLE startEvent = NULL;

    Create_critical_sections_and_events(massLock, startEvent);


    Marker obj_1(3, startEvent, &massLock, massive);
    Marker obj_2(2, startEvent, &massLock, massive);
    Marker obj_3(1, startEvent, &massLock, massive);

    Marker* obj_1_ptr = &obj_1;
    Marker* obj_2_ptr = &obj_2;
    Marker* obj_3_ptr = &obj_3;

    vector<Marker*> active_markers{ obj_1_ptr , obj_2_ptr, obj_1_ptr };
    int complete_thread_number = 2;

    int index = Find_complete_thread_index(active_markers, complete_thread_number);
    int expect_index = 1;
    BOOST_TEST(index == expect_index);
}
