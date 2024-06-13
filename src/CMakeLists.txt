# glob for source files
file(GLOB_RECURSE SOURCES "*.cpp")

# glob for header files
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE HEADERS
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
)
#define target
add_library(cppgl SHARED ${SOURCES} ${HEADERS})
generate_export_header(cppgl)

# public interface (cppgl.h is an available header file for all dependant projects)
target_include_directories(cppgl PUBLIC .)
target_include_directories(cppgl PRIVATE ../libs/glew/include)
target_include_directories(cppgl PRIVATE ../libs/glfw/include)
if (NOT assimp_FOUND)
    target_include_directories(cppgl PRIVATE ../libs/assimp-src/include)
else()
    target_include_directories(cppgl PRIVATE ${assimp_INCLUDE_DIR})
endif()

# ----------------------------------------------------------
# dependencies

# opengl
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_include_directories(cppgl PUBLIC ${OPENGL_INCLUDE_DIR} ${INCLUDE_DIR})
target_link_libraries(cppgl ${OPENGL_LIBRARIES})

target_link_libraries(cppgl stdc++fs) # required for std::filesystem


target_link_libraries(cppgl libglew_shared glfw assimp eigen)

source_group(
        TREE "${INCLUDE_DIR}"
        PREFIX "Header Files"
        FILES ${HEADERS}
)
source_group(
        TREE "${CMAKE_CURRENT_SOURCE_DIR}"
        PREFIX "Source Files"
        FILES ${SOURCES}
)