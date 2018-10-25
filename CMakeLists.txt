cmake_minimum_required(VERSION 2.6)
project(p2pnet)

add_executable(p2pnet main.cpp)

install(TARGETS p2pnet RUNTIME DESTINATION bin)
