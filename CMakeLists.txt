cmake_minimum_required(VERSION 3.6)

project(awEditor VERSION 0.0.1 DESCRIPTION "Small editor for the awRevolt engine")

include(dependencies/awRevolt/cmake/enableWarnings.cmake)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES)
set(HEADER_FILES)

add_subdirectory(source)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${TEST_FILES})

#dependencies

add_subdirectory(dependencies/awRevolt)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)

target_link_libraries(${PROJECT_NAME} awEngine awGUI OpenGL::GL)
