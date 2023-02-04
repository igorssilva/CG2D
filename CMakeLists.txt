cmake_minimum_required(VERSION 3.24)
project(CG2D)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB MyCSources *)

add_executable(CG2D ${MyCSources})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(CG2D ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
