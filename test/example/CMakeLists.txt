cmake_minimum_required(VERSION 3.0)

project(example)

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

include_directories(${GTEST_INCLUDE_DIRS})

find_library(
	EXAMPLE_LIBRARY 
	NAMES examplelibd 
	HINTS ${LIBDIR} 
	)

set(SRC_HEADER ${SRCDIR}/example/example.h)
file(GLOB SRCS *.cpp)
set(SRC_CPP ${SRCS} ${SRCDIR}/example/example.cpp)
set(SRC_FILES ${SRC_HEADER} ${SRC_CPP})

add_executable(${PROJECT_TEST_NAME} ${SRC_FILES})
target_link_libraries(
	${PROJECT_TEST_NAME} 
	gtest gtest_main gmock gmock_main
	${EXAMPLE_LIBRARY}
	)
add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
#add_test(NAME othername_example COMMAND test_example)

message(STATUS "example test is built")
