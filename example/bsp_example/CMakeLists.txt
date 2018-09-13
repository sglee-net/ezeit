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
	${SRCDIR}/bsp/BSPPoint.h 
	${SRCDIR}/bsp/BSPPointCollection.h 
	${SRCDIR}/bsp/BSPNode.h 
	${SRCDIR}/bsp/BSPTree.h
	)
set(SRC_CPP main.cpp)
set(SRC_FILES 
	${SRC_HEADER} 
#	${SRCDIR}/bsp/BSPPoint.cpp 
#	${SRCDIR}/bsp/BSPPointCollection.cpp 
#	${SRCDIR}/bsp/BSPNode.cpp 
#	${SRCDIR}/bsp/BSPTree.cpp 
	${SRC_CPP})

add_executable(bsp_example ${SRC_FILES})
target_link_libraries(bsp_example ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS}) 

message(STATUS "bsp_example is built")


