

add_executable(test_core test_core_number.c )
target_link_libraries(test_core CUT_lib)
add_executable(test_read test_read_file.c )
target_link_libraries(test_read CUT_lib)
add_test(test_cpu_core_num test_core)
add_test(test_read_file test_read)