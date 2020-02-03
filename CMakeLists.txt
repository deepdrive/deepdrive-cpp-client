cmake_minimum_required(VERSION 3.15)
project(deepdrive_cpp_client)

set(CMAKE_CXX_STANDARD 14)

#### Add ZMQ dependency
add_library(zmqpp SHARED IMPORTED)
set_target_properties(zmqpp PROPERTIES
  IMPORTED_LOCATION "/home/c2/src/zmqpp/build/max-g++/libzmqpp.so.4"
  INTERFACE_INCLUDE_DIRECTORIES "/home/c2/src/zmqpp/src/zmqpp"
)
find_package (Threads)
target_link_libraries(zmqpp INTERFACE ${CMAKE_THREAD_LIBS_INIT})
### End ZMQ


add_library(deepdrive_cpp_client SHARED deepdrive_client.cpp deepdrive_client.h)