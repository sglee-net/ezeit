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
	   gen-cpp/messenger_constants.cpp gen-cpp/messenger_constants.h  
	   gen-cpp/messenger_types.cpp gen-cpp/messenger_types.h 
	   gen-cpp/TransferService.cpp gen-cpp/TransferService.h
	   )

#add_custom_command(
#	OUTPUT ${THRIFT_SRC}
#	COMMAND thrift -r --gen cpp tutorial.thrift
#	)

#add_library(net ${THRIFT_SRC})

set(SRC_HEADER 
	${INCDIR}/glogger/glogger.h 
	${INCDIR}/glogger/gexecutor_filewriter.h 
	${INCDIR}/gtaskque/gtaskque.h
	)
set(SRC_CPP main.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP} ${SRC_THRIFT})

add_executable(thrift_example 
	${SRC_FILES})
target_link_libraries(thrift_example 
	${CMAKE_THREAD_LIBS_INIT} 
	${Boost_LIBRARIES}
	thrift)
#add_test(NAME test_example COMMAND test_example)

message(STATUS "thrifttest is built")

