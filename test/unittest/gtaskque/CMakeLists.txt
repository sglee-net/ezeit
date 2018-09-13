cmake_minimum_required(VERSION 3.0)

project(gtaskque)

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

#find_library(
#	EXAMPLE_LIBRARY 
#	NAMES examplelibd 
#	HINTS ${LIBDIR} 
#	)

file(GLOB SRCS *.cpp)
set(SRC_HEADER ${SRCDIR}/gtaskque/gtaskque.h)
set(SRC_CPP ${SRCS})
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})

add_executable(${PROJECT_TEST_NAME} ${SRC_FILES})
target_link_libraries(
	${PROJECT_TEST_NAME}
	gtest gtest_main gmock gmock_main
	# ${EXAMPLE_LIBRARY}
	)
add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})

message(STATUS "unit tests of gtaskque are built")
