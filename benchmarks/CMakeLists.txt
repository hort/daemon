find_package(benchmark REQUIRED)
if(benchmark_FOUND)

  file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/benchmarks/text.cpp)

  add_executable(bench ${SOURCES})

  target_link_libraries(bench benchmark)
  target_link_libraries(bench fmt)
  target_link_libraries(bench pthread)

else()
endif()
