cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0)

include(CTest)
enable_testing()

set( CMAKE_CXX_COMPILER "g++")
add_compile_options(-std=c++14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )
add_executable(testBuffer ./server/base/buffer.cpp ./server/base/test/testBuffer.cpp)
aux_source_directory(server/base SRC_BASE)
aux_source_directory(server/net SRC_NET)
aux_source_directory(server/exception SRC_EXCPT)

aux_source_directory(server/net/utils SRC_NET_UTILS)
aux_source_directory(server/net/httpRequest SRC_REQ)
aux_source_directory(server/net/session SRC_NET_SESSION)
aux_source_directory(application/src/controller SRC_CRL)

#add_executable(tcpServer ${SRC_BASE} ${SRC_NET}   server/net/test/testTcpServer)
#add_executable(httpServer ${SRC_BASE} ${SRC_NET} ${SRC_UTILS} ${SRC_REQ} ${SRC_EXCPT} server/net/test/testHttpServer)
add_library(myserver SHARED  ${SRC_BASE} ${SRC_NET} ${SRC_NET_SESSION} ${SRC_DATA} ${SRC_DATA_UTILS} ${SRC_NET_UTILS} ${SRC_REQ} ${SRC_EXCPT})


add_executable(httpApplication ${SRC_CRL} application/src/controller/testApplication)
target_link_libraries(httpApplication myserver)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
