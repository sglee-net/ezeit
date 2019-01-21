cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set(THRIFT_INCLUDE_DIR /usr/local/include/thrift)
set(THRIFT_LIB_DIR /usr/local/lib)

include_directories(${INCDIR})
include_directories(${SRCDIR})
link_directories(${LIBDIR})

include_directories(${THRIFT_INCLUDE_DIR})
link_directories(${THRIFT_LIB_DIR})

set(SRC_THRIFT
	   ${INCDIR}/thrift/messenger_constants.cpp 
	   ${INCDIR}/thrift/messenger_constants.h  
	   ${INCDIR}/thrift/messenger_types.cpp 
	   ${INCDIR}/thrift/messenger_types.h 
	   ${INCDIR}/thrift/ThriftRWService.cpp 
	   ${INCDIR}/thrift/ThriftRWService.h
	   )

#add_custom_command(
#	OUTPUT ${THRIFT_SRC}
#	COMMAND thrift -r --gen cpp tutorial.thrift
#	)

#add_library(net ${THRIFT_SRC})

set(SRC_HEADER 
	${INCDIR}/glogger/glogger.h 
	${INCDIR}/gexecutor/gexecutor_filewriter.h 
	${INCDIR}/gtaskque/gtaskque.h
	)
set(SRC_CPP main.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP} ${SRC_THRIFT})

add_executable(thriftclient 
	${SRC_FILES})
target_link_libraries(thriftclient 
	${CMAKE_THREAD_LIBS_INIT} 
	${Boost_LIBRARIES}
	thrift)
#add_test(NAME test_thriftclient COMMAND test_thriftclient)

message(STATUS "thriftclient is built")

