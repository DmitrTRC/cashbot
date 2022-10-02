cmake_minimum_required(VERSION 3.23)
project(cash_bot)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")
set(Boost_USE_MULTITHREADED ON)

set(ENV{OPENSSL_ROOT_DIR} "/opt/homebrew/opt/openssl@3")

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

include_directories(/usr/local/include ${OPENSSL_ROOT_DIR}/include ${Boost_INCLUDE_DIR} src)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()

add_subdirectory(lib/tgbot-cpp) # Telegram API


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
message(STATUS "cash_bot Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


### 3-d Party Libraries
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(sqlite URL https://github.com/fnc12/sqlite_orm/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(sqlite)
### Build executable

set(SOURCES
        src/main.cpp
        src/Categories.cpp
        src/db.cpp
        src/Env_Keeper.cpp
        src/Expense.cpp
        src/Flow_Bot.cpp
        src/Auth.cpp)

add_executable(
        cash_bot
        ${SOURCES}
)

target_include_directories(cash_bot
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

target_link_libraries(cash_bot TgBot
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
        nlohmann_json::nlohmann_json
        sqlite_orm::sqlite_orm
        )