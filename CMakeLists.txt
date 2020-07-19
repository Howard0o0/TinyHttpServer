#设置cmake版本号
cmake_minimum_required(VERSION 3.4.0)
 
#设置工程名称
project(tinythreadpool)
 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 
add_definitions(-std=c++11)
set(CXX_FLAGS
 -g
 -Wall
 -Wextra
 # -m32
 #-Werror
 #-Wconversion
 -Wno-unused-parameter
 #-Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 -rdynamic
 )
set (MUDUO_PATH "${CMAKE_SOURCE_DIR}/third-party-libs/muduo")
find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})
message(STATUS ${Muduo_INCLUDE_DIR})
message(STATUS ${Muduo_LIBRARY_DIR})

include_directories(${Muduo_INCLUDE_DIR})
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)
message(STATUS ${muduo_base})
message(STATUS ${muduo_net})

 
#find head files
include_directories( ./src) 

link_directories(${MUDUO_PATH}/lib)

#find source files
aux_source_directory(./src SRC_FILES)

#创建工程文件
add_executable(tinythreadpool.app ${SRC_FILES} )

target_link_libraries(tinythreadpool.app ${muduo_net} ${muduo_base} pthread )

 