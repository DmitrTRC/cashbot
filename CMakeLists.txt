cmake_minimum_required(VERSION 3.23)
project(bot_lib)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

set(Boost_USE_MULTITHREADED ON)

set(ENV{OPENSSL_ROOT_DIR} "/opt/homebrew/opt/openssl@3")

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(CURL)

include_directories(/usr/local/include ${OPENSSL_ROOT_DIR}/include ${Boost_INCLUDE_DIR})

include_directories(${CURSES_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
message(STATUS "cash_bot Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


### 3-d Party Libraries
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(sqlite URL https://github.com/fnc12/sqlite_orm/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(sqlite)

FetchContent_Declare(tgbot URL https://github.com/reo7sp/tgbot-cpp/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(tgbot)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(bot_lib STATIC
            src/Categories.cpp
            src/db.cpp
            src/Env_Keeper.cpp
            src/Expense.cpp
            src/Flow_Bot.cpp
            src/Bot_Exceptions.cpp
            )

target_include_directories(bot_lib
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}/include
                           )

target_link_libraries(bot_lib
                      PUBLIC
                      TgBot
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${OPENSSL_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${CURL_LIBRARIES}
                      nlohmann_json::nlohmann_json
                      sqlite3
                      )

############################################################
# Create an executable
############################################################

add_executable(
        bot_binary
        App/main.cpp
)


target_link_libraries(bot_binary PRIVATE
                      bot_lib)

######### TEST SECTION ###########


add_subdirectory(tests)