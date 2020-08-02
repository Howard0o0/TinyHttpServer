#设置cmake版本号
cmake_minimum_required(VERSION 3.4.0)
 
#设置工程名称
project(tinythreadpool)

#option(build_men_client "build men client" ON)
 

#pack in deb info


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
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
set (CURL_PATH "${CMAKE_SOURCE_DIR}/third-party-libs/curl")
set (HIREDIS_PATH "${CMAKE_SOURCE_DIR}/third-party-libs/hiredis")
set (SPDLOG_PATH "${CMAKE_SOURCE_DIR}/third-party-libs/spdlog")

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
include_directories( ./src/base) 
include_directories( ./src/net) 
include_directories( ./src/men) 
include_directories( ./src/men-client) 
include_directories (${CURL_PATH}/include)
include_directories (${HIREDIS_PATH}/include)
include_directories (${SPDLOG_PATH}/include)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

link_directories(/usr/local/lib ${MUDUO_PATH}/lib ${CURL_PATH}/lib ${HIREDIS_PATH}/lib)
 

#find source files
aux_source_directory(./src SRC_FILES)
aux_source_directory(./src/base SRC_BASE_FILES)
aux_source_directory(./src/net SRC_NET_FILES)
aux_source_directory(./src/men SRC_MEN_FILES)
aux_source_directory(./src/men-client SRC_MENCLIENT_FILES)


add_executable(men-client.app  ${SRC_BASE_FILES} ${SRC_NET_FILES} ${SRC_MENCLIENT_FILES})
target_link_libraries(men-client.app ${muduo_net} ${muduo_base} pthread curl boost_locale re2 hiredis)
	
add_executable(men-server.app ${SRC_FILES} ${SRC_BASE_FILES} ${SRC_NET_FILES} ${SRC_MEN_FILES})
target_link_libraries(men-server.app ${muduo_net} ${muduo_base} pthread curl boost_locale re2 hiredis)
 