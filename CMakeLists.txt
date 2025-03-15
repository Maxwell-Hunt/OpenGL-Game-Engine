cmake_minimum_required(VERSION 3.22.1)
project(my_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "external/glad/glad.c" "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/external/glad" "${CMAKE_SOURCE_DIR}/external/stbimage")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)

add_subdirectory(external/assimp)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm::glm-header-only)
target_link_libraries(${PROJECT_NAME} assimp)