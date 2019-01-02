cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)
add_definitions(-D__linux__)
add_definitions(-D__GNU_G__)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCDIR})
include_directories(${SRCDIR})

find_package(OpenCV)
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

#link_directories(${NIB} /usr/local/lib)

set(SRC_HEADER 
	${INCDIR}/glogger/glogger.h 
	${INCDIR}/gexecutor/gexecutor_filewriter.h 
	${INCDIR}/gtaskque/gtaskque.h
	)
set(SRC_CPP main.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})

add_executable(opencv_example ${SRC_FILES})
target_link_libraries(opencv_example ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS}) 

message(STATUS "opencv_example is built")


