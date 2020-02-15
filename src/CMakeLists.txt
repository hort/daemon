file(GLOB SOURCES
  # http
  ${PROJECT_SOURCE_DIR}/src/hort/http/cookiejar.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/http/form.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/http/session.cpp

  # utilities
  ${PROJECT_SOURCE_DIR}/src/hort/archive.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/filesystem.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/formats.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/fzf.cpp

  # infrastructure
  ${PROJECT_SOURCE_DIR}/src/hort/index.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/interface.cpp
  ${PROJECT_SOURCE_DIR}/src/hort/re.cpp

  ${PROJECT_SOURCE_DIR}/src/hort/main.cpp)

set(HEADERS
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external)

add_executable(hortd ${SOURCES})
target_include_directories(hortd PRIVATE ${HEADERS})
target_compile_options(hortd PRIVATE -Wall -Wextra)

# mongocxx
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
include_directories(${LIBBSONCXX_INCLUDE_DIRS})
include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
target_link_libraries(hortd mongocxx)
target_link_libraries(hortd bsoncxx)

# metrics
#target_link_libraries(hortd prometheus-cpp-core)
#target_link_libraries(hortd prometheus-cpp-pull)

# logging
#target_link_libraries(hortd loki-cpp)

# misc
target_link_libraries(hortd curl)
target_link_libraries(hortd fmt)
target_link_libraries(hortd pthread)
target_link_libraries(hortd re2)
target_link_libraries(hortd yaml-cpp)
target_link_libraries(hortd zip)
