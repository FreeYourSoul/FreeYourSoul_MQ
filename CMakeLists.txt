cmake_minimum_required(VERSION 3.3)
project(FreeSouls CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_timer -lboost_system -lboost_thread -pthread -Wall -Wpedantic -Wextra ")

set(CMAKE_CXX_COMPILER /usr/bin/g++)

include_directories(include include/bus include/queue)

set(SOURCE_FILES main.cpp ${SOURCE_FILES_MQ})
set(SOURCE_FILES_MQ src/bus/FysBus.cpp include/bus/FysBus.hh include/queue/LockFreeQueue.hh include/queue/QueueContainer.hh include/queue/LockingQueue.hh)
set(SOURCE_FILES_TEST ${SOURCE_FILES_MQ} test/FySBusTestCase.cpp)

### Exexutable of test ###
add_executable(FreeSouls ${SOURCE_FILES})

### Linked Library compiled ###
#add_library( )

# link against dynamic libraries
add_definitions(-DBOOST_ALL_DYN_LINK)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

## Test executable
add_executable(UnitTests ${SOURCE_FILES_TEST})
target_link_libraries(UnitTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})