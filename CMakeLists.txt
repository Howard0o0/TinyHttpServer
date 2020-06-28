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
 
#find head files
include_directories( ./src) 

#find source files
aux_source_directory(./src SRC_FILES)

#创建工程文件
add_executable(tinythreadpool.app ${SRC_FILES} )

target_link_libraries(tinythreadpool.app pthread)

 