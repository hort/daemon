file(GLOB SOURCES
  # interfaces
  ${PROJECT_SOURCE_DIR}/src/interfaces/*

  # http
  ${PROJECT_SOURCE_DIR}/src/hort/http/cookiejar.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/http/session.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/http/form.cpp

  # term
  ${PROJECT_SOURCE_DIR}/src/hort/term/fzf.cpp

  # library
  ${PROJECT_SOURCE_DIR}/src/hort/archive.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/filesystem.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/formats.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/re.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/string.cpp

  # infrastructure
  ${PROJECT_SOURCE_DIR}/src/hort/index.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/args.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/registry.cpp

  # third party
  ${PROJECT_SOURCE_DIR}/third_party/md5.cpp

  ${PROJECT_SOURCE_DIR}/src/main.cpp)

set(HEADERS
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/third_party)

add_executable(hortd ${SOURCES})
target_include_directories(hortd PRIVATE ${HEADERS})
target_compile_options(hortd PRIVATE -Wall -Wextra -Wpedantic -ggdb)

# mongocxx
find_package(libmongocxx)
find_package(libbsoncxx)
if(libbsoncxx_FOUND AND libmongocxx_FOUND)
    include_directories(
      ${LIBMONGOCXX_INCLUDE_DIRS}
      ${LIBBSONCXX_INCLUDE_DIRS})
    target_link_libraries(hortd
      ${LIBMONGOCXX_LIBRARIES}
      ${LIBBSONCXX_LIBRARIES})
else()
  find_package(libmongocxx-static)
  find_package(libbsoncxx-static)
  if(libbsoncxx-static_FOUND AND libmongocxx-static_FOUND)
    include_directories(
      ${LIBMONGOCXX_STATIC_INCLUDE_DIRS}
      ${LIBBSONCXX_STATIC_INCLUDE_DIRS})
    target_link_libraries(hortd
      ${LIBMONGOCXX_STATIC_LIBRARIES}
      ${LIBBSONCXX_STATIC_LIBRARIES})
  else()
    message("error")
  endif()
endif()

# metrics
#target_link_libraries(hortd prometheus-cpp-core)
#target_link_libraries(hortd prometheus-cpp-pull)

# logging
target_link_libraries(hortd loki-cpp)

# misc
target_link_libraries(hortd curl)
target_link_libraries(hortd fmt)
target_link_libraries(hortd pthread)
target_link_libraries(hortd re2)
target_link_libraries(hortd yaml-cpp)
target_link_libraries(hortd zip)

add_custom_command(TARGET hortd POST_BUILD
  COMMAND notify-send "ninja: ${CMAKE_PROJECT_NAME}" "Compilation Terminated" &
)
