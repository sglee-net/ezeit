cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)

project(gexecutorthrift)

set(THRIFT_INCLUDE_DIR /usr/local/include/thrift)
set(THRIFT_LIB_DIR /usr/local/lib)

include_directories(${THRIFT_INCLUDE_DIR})
link_directories(${THRIFT_LIB_DIR})

set(SRC_THRIFT
           ${INCDIR}/thrift/messenger_constants.cpp 
	   ${INCDIR}/thrift/messenger_constants.h  
           ${INCDIR}/thrift/messenger_types.cpp 
	   ${INCDIR}/thrift/messenger_types.h 
           ${INCDIR}/thrift/ThriftService.cpp 
	   ${INCDIR}/thrift/ThriftService.h
           )

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

#find_library(
#	EXAMPLE_LIBRARY 
#	NAMES examplelibd 
#	HINTS ${LIBDIR} 
#	)

file(GLOB SRCS *.cpp)
set(SRC_HEADER ${SRCDIR}/gtaskque/gtaskque.h 
		${SRCDIR}/gexecutor/gexecutor_thrift_writemessage.h)
set(SRC_CPP ${SRCS})
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP} ${SRC_THRIFT})

add_executable(${PROJECT_TEST_NAME} ${SRC_FILES})
target_link_libraries(
	${PROJECT_TEST_NAME}
	gtest gtest_main gmock gmock_main
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	thrift
	# ${EXAMPLE_LIBRARY}
	)
add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})

message(STATUS "unit tests of gexecutorthrift are built")
