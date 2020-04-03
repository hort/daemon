include(GoogleTest)

set(HEADERS
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/third_party
  ${PROJECT_SOURCE_DIR}/external)

add_executable(string string.cpp)
target_include_directories(string PRIVATE ${HEADERS})
target_link_libraries(string gtest)
target_link_libraries(string fmt)

gtest_add_tests(TARGET string)
