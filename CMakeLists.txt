cmake_minimum_required(VERSION 2.6)
project(graphics)
include_directories(include)
file(GLOB headers
    cmake/modules/*
    include/andres/*.hxx
    include/andres/graphics/*.hxx
)
enable_testing()

##############################################################################
# GNU g++ specific settings
##############################################################################
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

##############################################################################
# HDF5
##############################################################################
find_package(HDF5)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})
endif()

##############################################################################
# OpenGL
##############################################################################
find_package(OpenGL)
if(OpenGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
endif()

##############################################################################
# GLUT
##############################################################################
find_package(GLUT)
if(GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIR})
endif()

##############################################################################
# targets: graphics
##############################################################################
if(GLUT_FOUND AND HDF5_FOUND)
    add_executable(test-graphics-hdf5 src/andres/graphics/unittest/graphics-hdf5.cxx ${headers})
    target_link_libraries(test-graphics-hdf5 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${HDF5_LIBRARIES})
    add_test(test-graphics-hdf5 test-graphics-hdf5)

    add_executable(viewer-opengl src/andres/graphics/viewer-opengl.cxx ${headers})
    target_link_libraries(viewer-opengl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${HDF5_LIBRARIES})
endif()

