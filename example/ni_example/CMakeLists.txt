cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)
#add_definitions(-D__linux)
add_definitions(-D__linux__)
#add_definitions(-D__int64="long long")
add_definitions(-D__GNU_G__)
#add_definitions(-D__x86_64__)

set(NIROOT /cygdrive/c/Program\ Files\ \(x86\)/National\ Instruments/Shared/ExternalCompilerSupport/C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${INCDIR})
include_directories(${SRCDIR})
include_directories(${NIROOT}/include)
#link_directories(${NIB} /usr/local/lib)
#find_library(NIB 
#	NAMES nidaqmx 
#	HINTS ${NIROOT}/lib64/gcc)
#link_directories(${NIB} ${NIROOT}/lib64/gcc)
link_directories(${NIB} ${NIROOT}/lib64/msvc)

set(SRC_HEADER ${INCDIR}/glogger/glogger.h ${INCDIR}/glogger/gexecutor_filewriter.h ${INCDIR}/gtaskque/gtaskque.h ${NIROOT}/include/NIDAQmx.h)
set(SRC_CPP main.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})

add_executable(ni_example ${SRC_FILES})
#target_link_libraries(ni_example ${CMAKE_THREAD_LIBS_INIT} nisyscfg) 
target_link_libraries(ni_example ${CMAKE_THREAD_LIBS_INIT} nidaqmx) 
#target_link_libraries(ni_example ${CMAKE_THREAD_LIBS_INIT} nidaqmx)
#add_test(NAME test_example COMMAND test_example)

message(STATUS "ni_example is built")

