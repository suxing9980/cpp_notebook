cmake_minimum_required(VERSION 3.15)
project(SyncQueue)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp SyncQueue.h)

add_executable(SyncQueue ${SOURCE_FILES})