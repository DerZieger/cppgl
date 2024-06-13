# target name to generate here
set(TARGET example)

# glob source files
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

# define target
add_executable(${TARGET} ${SOURCES} ${HEADERS})

# forces executables to be compiled to /example/ folder, to allow relative paths for shaders
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# built libs
target_link_libraries(${TARGET} cppgl)
