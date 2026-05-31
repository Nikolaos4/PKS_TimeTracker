cmake_minimum_required(VERSION 3.10)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
pkg_check_modules(POSTGRES REQUIRED libpq)

add_executable(timetracker
    src/main.cpp
    src/User.cpp
    src/Task.cpp
    src/TimeEntry.cpp
    src/Notification.cpp
    src/DatabaseFacade.cpp
    src/Observer.cpp
    src/hash_utils.cpp  
)

target_include_directories(timetracker PRIVATE include)
target_include_directories(timetracker PRIVATE ${PQXX_INCLUDE_DIRS} ${POSTGRES_INCLUDE_DIRS})
target_link_libraries(timetracker ${PQXX_LIBRARIES} ${POSTGRES_LIBRARIES} ws2_32 wsock32)

enable_testing()
#add_subdirectory(tests)