cmake_minimum_required(VERSION 3.0.0)
project(serverLib VERSION 0.1.0)

include(CTest)
enable_testing()

set( CMAKE_CXX_COMPILER "g++")
add_compile_options(-std=c++14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARY NAMES hiredis)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(serverLib/base)
add_subdirectory(serverLib/net)
aux_source_directory(serverLib/base base_SRCS)
aux_source_directory(serverLib/net/tcp tcp_SRCS)
aux_source_directory(serverLib/net/utils utils_SRCS)
aux_source_directory(serverLib/net/http http_SRCS)

add_library(serverLib ${base_SRCS} ${tcp_SRCS} ${http_SRCS} ${utils_SRCS})

target_link_libraries(serverLib hiredis)
install(TARGETS serverLib
ARCHIVE DESTINATION lib)

set(HEADERS
serverLib/http
serverLib/tcp
)
install(FILES ${HEADERS} DESTINATION include/serverLib)

# add_subdirectory(example/httpServer)