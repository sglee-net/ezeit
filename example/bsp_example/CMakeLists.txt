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
	${INCDIR}/glogger/gexecutor_filewriter.h 
	${INCDIR}/gtaskque/gtaskque.h 
	${SRCDIR}/bsp/QuadTreePoint.h 
	${SRCDIR}/bsp/QuadTreePointCollection.h 
	${SRCDIR}/bsp/QuadTreeNode.h 
	${SRCDIR}/bsp/QuadTree.h
	)
set(SRC_CPP main.cpp)
set(SRC_FILES 
	${SRC_HEADER} 
#	${SRCDIR}/bsp/QuadTreePoint.cpp 
#	${SRCDIR}/bsp/QuadTreePointCollection.cpp 
#	${SRCDIR}/bsp/QuadTreeNode.cpp 
#	${SRCDIR}/bsp/QuadTree.cpp 
	${SRC_CPP})

add_executable(bsp_example ${SRC_FILES})
target_link_libraries(bsp_example ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS}) 

message(STATUS "bsp_example is built")


