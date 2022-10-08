### Add Tests

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(bot_test
               db_test.cpp
               )

target_include_directories(bot_test PRIVATE
                           ${CMAKE_SOURCE_DIR}/src
                           )

target_link_libraries(bot_test
                      bot_lib
                      GTest::gtest_main
                      )

include(GoogleTest)
gtest_discover_tests(bot_test)