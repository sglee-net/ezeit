cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCDIR})
include_directories(${SRCDIR})
link_directories(${LIBDIR} /usr/local/lib)
#find_library(NILIB NAMES libnidaqmx.so HINTS ${NIROOT}/lib) #REQUIRED

set(SRC_HEADER 
	${INCDIR}/glogger/glogger.h 
	${INCDIR}/gexecutor/gexecutor_filewriter.h 
	${INCDIR}/gtaskque/gtaskque.h)
set(SRC_CPP main.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})

add_executable(glogger_write_log ${SRC_FILES})
target_link_libraries(glogger_write_log ${CMAKE_THREAD_LIBS_INIT}) # ${NILIB})
#add_test(NAME test_example COMMAND test_example)

message(STATUS "glogger_write_log is built")

