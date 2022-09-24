cmake_minimum_required(VERSION 3.22.1)
project("hellonnapi")

# NOTE: This seems to not work when used from gradle;
# It's also set in build.gradle and that seems work
set (CMAKE_CXX_STANDARD 17)

add_library(hellonnapi SHARED
        hellonnapi.cpp)

find_library(nnlibs neuralnetworks REQUIRED)
find_library(androidlibs android REQUIRED)
find_library(loglibs log REQUIRED)

target_link_libraries(hellonnapi ${nnlibs} ${androidlibs} ${loglibs})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
