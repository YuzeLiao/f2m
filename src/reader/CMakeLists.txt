# Build library reader
add_library(reader reader.cc)

# Build uinttests.
set(LIBS reader gtest base)

add_executable(parser_test parser_test.cc)
target_link_libraries(parser_test gtest_main ${LIBS})

add_executable(reader_test reader_test.cc)
target_link_libraries(reader_test gtest_main ${LIBS})

# Install library and header files
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/reader)
