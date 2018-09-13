cmake_minimum_required(VERSION 3.0)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_DEBUG")

set(SRC_THRIFT
	gen-cpp/messenger_constants.cpp gen-cpp/messenger_constants.h
	gen-cpp/messenger_types.cpp gen-cpp/messenger_types.h
	gen-cpp/TransferService.cpp gen-cpp/TransferService.h
	)

set(SRC_HEADER ./glogger.h ./gexecutor_filewriter.h ./gexecutor_thrift.h)
set(SRC_FILES ${SRC_HEADER} ${SRC_THRIFT})
set(CURRDIR glogger)

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURRDIR}) 
