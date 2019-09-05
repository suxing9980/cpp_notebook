cmake_minimum_required(VERSION 3.14)
project(cppzmq_learning)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp ZmqCommunication.cpp ZmqCommunication.h)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cppzmq)
#set(LINK_DIR /server/build/lib)

include_directories(${INC_DIR})
#link_directories(${LINK_DIR})
#link_libraries(rdkafka++)

add_executable(cppzmq_learning ${SOURCE_FILES})
#target_link_libraries(testLibRdkafka rdkafka++ pthread)