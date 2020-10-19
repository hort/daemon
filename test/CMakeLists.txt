find_package(GTest)

if(GTEST_FOUND)
  include(GoogleTest)

  set(HEADERS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party)

  file(GLOB SOURCES
    # Library
    ${PROJECT_SOURCE_DIR}/src/hort/filesystem.cpp
    ${PROJECT_SOURCE_DIR}/src/hort/string.cpp
    ${PROJECT_SOURCE_DIR}/src/hort/re.cpp

    # Unit Tests
    main.cpp

    string.cpp
    vector.cpp
    text.cpp
    filesystem.cpp)

  add_executable(unittest ${SOURCES})
  target_include_directories(unittest PRIVATE ${HEADERS})
  target_link_libraries(unittest gtest)
  target_link_libraries(unittest fmt)
  target_link_libraries(unittest re2)

  gtest_add_tests(TARGET unittest)
endif()
