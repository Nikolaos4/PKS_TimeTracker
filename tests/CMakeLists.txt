include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(Catch2)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)


add_executable(test_user test_user.cpp ../src/User.cpp)
target_link_libraries(test_user PRIVATE Catch2::Catch2WithMain)
target_include_directories(test_user PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME test_user COMMAND test_user)

add_executable(test_task test_task.cpp ../src/Task.cpp)
target_link_libraries(test_task PRIVATE Catch2::Catch2WithMain)
target_include_directories(test_task PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME test_task COMMAND test_task)

add_executable(test_timeentry test_timeentry.cpp ../src/TimeEntry.cpp)
target_link_libraries(test_timeentry PRIVATE Catch2::Catch2WithMain)
target_include_directories(test_timeentry PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME test_timeentry COMMAND test_timeentry)

add_executable(test_notification test_notification.cpp ../src/Notification.cpp)
target_link_libraries(test_notification PRIVATE Catch2::Catch2WithMain)
target_include_directories(test_notification PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME test_notification COMMAND test_notification)

add_executable(test_databasefacade 
    test_databasefacade.cpp 
    ../src/User.cpp
    ../src/Task.cpp
    ../src/TimeEntry.cpp
    ../src/Notification.cpp
    ../src/DatabaseFacade.cpp
)
target_link_libraries(test_databasefacade PRIVATE Catch2::Catch2WithMain)
target_include_directories(test_databasefacade PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME test_databasefacade COMMAND test_databasefacade)

add_executable(scenarios 
    scenarios.cpp 
    ../src/User.cpp
    ../src/Task.cpp
    ../src/TimeEntry.cpp
    ../src/Notification.cpp
    ../src/DatabaseFacade.cpp
)
target_link_libraries(scenarios PRIVATE Catch2::Catch2WithMain)
target_include_directories(scenarios PRIVATE ${PROJECT_INCLUDE_DIR})
add_test(NAME scenarios COMMAND scenarios)