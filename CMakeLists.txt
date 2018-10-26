cmake_minimum_required(VERSION 2.6)
project(p2pnet)

# enforce C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)


add_executable(p2pnet peer.cpp out_peer_factory.cpp inc_peer_factory.cpp CMakeLists.txt main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(p2pnet Threads::Threads)

install(TARGETS p2pnet RUNTIME DESTINATION bin)
