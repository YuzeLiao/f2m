# Build unittests.
set(LIBS data base gtest)

add_executable(logit_loss_test logit_loss_test.cc)
target_link_libraries(logit_loss_test gtest_main ${LIBS})

# Install library and header files
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/loss)