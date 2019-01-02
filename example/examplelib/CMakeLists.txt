cmake_minimum_required(VERSION 3.0)

include_directories(${INCDIR})
link_directories(${LIBDIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_HEADER ./examplelib.h)
set(SRC_CPP ./examplelib.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})
set(CURR_DIR examplelib)

# create library called "examplelib"
add_library(examplelib SHARED ${SRC_FILES})

install(FILES ${SRC_HEADER} DESTINATION ${INCDIR}/${CURR_DIR}) 
install(TARGETS examplelib DESTINATION ${LIBDIR})
