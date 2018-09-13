cmake_minimum_required(VERSION 3.0)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_BUILD_TYPE Debug)

#set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_DEBUG")

set(SRC_HEADER utbyte.hpp)
set(SRC_FILES ${SRC_HEADER})
set(CURRDIR utbyte)

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURRDIR}) 
