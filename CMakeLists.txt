cmake_minimum_required(VERSION 2.8)
project(ARTool)
find_package(OpenCV REQUIRED)
add_executable(ARTool main.cpp classes/Image.cpp classes/Image.h classes/Geometry.cpp classes/Geometry.h classes/Util.cpp classes/Util.h classes/MarkerDetector.cpp classes/MarkerDetector.h)

add_executable(CalibrationTool opencv_calibration.cpp)


# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ) # ${GLUT_INCLUDE_DIRS}

# target_link_libraries(ARTool ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries(ARTool ${OpenCV_LIBS})
target_link_libraries(CalibrationTool ${OpenCV_LIBS})