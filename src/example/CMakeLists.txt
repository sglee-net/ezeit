cmake_minimum_required(VERSION 3.0)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_HEADER ./example.h)
set(CURR_DIR example)

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURR_DIR}) 
