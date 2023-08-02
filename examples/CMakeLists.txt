add_executable(find_error find_error.cpp)
target_link_libraries(find_error PUBLIC lust)

add_executable(bench bench.cpp)
target_link_libraries(bench PUBLIC lust)


