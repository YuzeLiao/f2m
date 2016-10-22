# Build library data
add_library(data model_parameters.cc)

# Install library and header files
install(TARGETS data DESTINATION lib/data)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/data)
