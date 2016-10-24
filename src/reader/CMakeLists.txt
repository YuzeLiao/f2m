# Build library reader

# Build uinttests.
set(LIBS data gtest base)

add_executable(parser_test parser_test.cc)
target_link_libraries(parser_test gtest_main ${LIBS})

# Install library and header files

