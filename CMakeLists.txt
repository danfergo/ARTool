cmake_minimum_required(VERSION 2.8)
project(ARTool)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

add_executable(ARTool ar.cpp util/Image.cpp util/Image.h util/Geometry.cpp util/Geometry.h util/Util.cpp util/Util.h classes/MarkerDetector.cpp classes/MarkerDetector.h classes/CameraCalibrator.cpp classes/CameraCalibrator.h util/Camera.cpp util/Camera.h classes/MarkerDescriptor.cpp classes/MarkerDescriptor.h util/Shape.cpp util/Shape.h classes/Database.cpp classes/Database.h)

add_executable(CalibrationTool calibrator.cpp util/Image.cpp util/Image.h util/Geometry.cpp util/Geometry.h util/Util.cpp util/Util.h classes/MarkerDetector.cpp classes/MarkerDetector.h classes/CameraCalibrator.cpp classes/CameraCalibrator.h util/Camera.cpp util/Camera.h classes/MarkerDescriptor.cpp classes/MarkerDescriptor.h)



# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ) # ${GLUT_INCLUDE_DIRS}

# target_link_libraries(ARTool ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries(ARTool ${OpenCV_LIBS})
target_link_libraries(CalibrationTool ${OpenCV_LIBS})