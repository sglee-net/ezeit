cmake_minimum_required(VERSION 3.0)

find_package(Threads)
add_definitions(-Dpthread)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_HEADER ./gsubf.h)
set(SRC_CPP ./main.cpp ./gsubf.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})
set(CURR_DIR gsubf)

add_executable(gsubf ${SRC_FILES})
target_link_libraries(gsubf ${CMAKE_THREAD_LIBS_INIT})
#add_library(gsubf SHARED ${SRC_FILES})

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURR_DIR}) 
install(TARGETS gsubf DESTINATION ${BINDIR})
