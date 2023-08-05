add_executable(find_error find_error.cpp)
target_link_libraries(find_error PUBLIC lust)
target_compile_options(find_error PRIVATE -Wall -Wextra -pedantic)

add_executable(bench bench.cpp)
target_link_libraries(bench PUBLIC lust)
target_compile_options(bench PRIVATE -Wall -Wextra -pedantic)


