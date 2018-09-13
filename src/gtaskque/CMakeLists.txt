cmake_minimum_required(VERSION 3.0)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_HEADER ./gtaskque.h)
set(SRC_FILES ${SRC_HEADER})
set(CURRDIR gtaskque)

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURRDIR}) 
