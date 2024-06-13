cmake_minimum_required(VERSION 3.16)

PROJECT(CppGL
    VERSION 1.8
    DESCRIPTION "OpenGL framework for use in teaching and research with focus on simplicity and reusability."
    LANGUAGES CXX
)

if (NOT UNIX )
	message(FATAL_ERROR "Unsupported platform!")
endif()

# ---------------------------------------------------------------------
# cmake options

option(CPPGL_BUILD_EXAMPLES "" OFF)

# ---------------------------------------------------------------------
# path management

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

# ---------------------------------------------------------------------
# subtrees

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# ---------------------------------------------------------------------
# fetch content

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)


FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG master)
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
#set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)
#target_include_directories(3d_renderer PRIVATE libs/assimp-src/include)
#target_link_libraries(3d_renderer assimp)

FetchContent_Declare(
        Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
#set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)

FetchContent_MakeAvailable(Eigen)

#target_link_libraries(YourTarget PRIVATE Eigen3::Eigen)



FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
    set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
    set(GLFW_USE_CHDIR OFF CACHE BOOL " " FORCE)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()
FetchContent_MakeAvailable(glfw)
#target_link_libraries(game glfw )

FetchContent_Declare(glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG master)
set(glew-cmake_BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_SHARED ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glew)


# ---------------------------------------------------------------------
# compiler options

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# setup for included dependencies
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
set(CMAKE_LINK_DIRECTORIES_BEFORE ON)


set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wno-unused-parameter -Wno-unused-variable -Wno-int-to-pointer-cast -march=native -funroll-loops -finline-functions")
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations -pthread")

# linker flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GenerateExportHeader)

# ---------------------------------------------------------------------
# traverse source tree

add_subdirectory(src)

if (CPPGL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
