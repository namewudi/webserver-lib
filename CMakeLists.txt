cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0)

include(CTest)
enable_testing()

set( CMAKE_CXX_COMPILER "g++")
add_compile_options(-std=c++14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )
add_executable(testBuffer ./server/base/buffer.cpp ./server/base/test/testBuffer.cpp)
aux_source_directory(server/base/ SRC_BASE)
aux_source_directory(server/net/ SRC_NET)

add_executable(tcpServer ${SRC_BASE} ${SRC_NET} server/net/test/testTcpServer)
add_executable(httpServer ${SRC_BASE} ${SRC_NET} server/net/test/testHttpServer)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
