include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(Catch2)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Находим библиотеки PostgreSQL и libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
pkg_check_modules(POSTGRES REQUIRED libpq)

# Функция для добавления тестов
function(add_test_executable NAME SOURCE_FILE)
    add_executable(${NAME} ${SOURCE_FILE}
        ${PROJECT_SRC_DIR}/User.cpp
        ${PROJECT_SRC_DIR}/Task.cpp
        ${PROJECT_SRC_DIR}/TimeEntry.cpp
        ${PROJECT_SRC_DIR}/Notification.cpp
        ${PROJECT_SRC_DIR}/DatabaseFacade.cpp
        ${PROJECT_SRC_DIR}/Observer.cpp
        ${PROJECT_SRC_DIR}/hash_utils.cpp
    )
    target_link_libraries(${NAME} PRIVATE Catch2::Catch2WithMain ${PQXX_LIBRARIES} ${POSTGRES_LIBRARIES} ws2_32 wsock32)
    target_include_directories(${NAME} PRIVATE ${PROJECT_INCLUDE_DIR} ${PQXX_INCLUDE_DIRS} ${POSTGRES_INCLUDE_DIRS})
    add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

# Добавляем тесты
add_test_executable(test_user test_user.cpp)
add_test_executable(test_task test_task.cpp)
add_test_executable(test_timeentry test_timeentry.cpp)
add_test_executable(test_notification test_notification.cpp)
add_test_executable(test_databasefacade test_databasefacade.cpp)
add_test_executable(scenarios scenarios.cpp)