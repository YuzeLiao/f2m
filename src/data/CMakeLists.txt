# Build library data
add_library(data model_parameters.cc)

# Build unittests.
set(LIBS data base gtest)

add_executable(model_parameters_test model_parameters_test.cc)
target_link_libraries(model_parameters_test gtest_main ${LIBS})

# Install library and header files
install(TARGETS data DESTINATION lib/data)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/data)
