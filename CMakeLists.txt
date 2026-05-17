cmake_minimum_required(VERSION 3.10)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(timetracker
    src/main.cpp
    src/User.cpp
    src/Task.cpp
    src/TimeEntry.cpp
    src/Notification.cpp
    src/DatabaseFacade.cpp
)


target_include_directories(timetracker PRIVATE include)

enable_testing() #включить поддержку тестов (для CTest).
add_subdirectory(tests) #подключить папку tests, в которой будет свой CMakeLists.txt