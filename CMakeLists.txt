cmake_minimum_required(VERSION 3.10)

# Project Name
project(Ronin)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Determine build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/debug)
    set(DLL_PATH ${CMAKE_SOURCE_DIR}/bin/debug)
    set(DEST_BIN_PATH ${CMAKE_BINARY_DIR}/bin/Debug)
else()
    set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/release)
    set(DLL_PATH ${CMAKE_SOURCE_DIR}/bin/release)
    set(DEST_BIN_PATH ${CMAKE_BINARY_DIR}/bin/Release)
endif()

# Link directories
link_directories(${LIBRARY_PATH})

# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/main.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libraries
# Example replacement of YourLibName with actual library names
target_link_libraries(${PROJECT_NAME} ControlCAN ECanVci64)

# Ensure DLLs are copied to the output directory
file(GLOB_RECURSE DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.dll")

foreach(DLL_FILE ${DLL_FILES})
    get_filename_component(FILENAME ${DLL_FILE} NAME)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_PATH}/${FILENAME} ${DEST_BIN_PATH}
        COMMENT "Copying DLLs to output directory"
    )
endforeach()

# Optional: Set the C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)