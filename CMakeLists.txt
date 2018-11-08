cmake_minimum_required(VERSION 2.6)
project(p2pnet)

# enforce C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(p2pnet p2p.cpp epee/src/memwipe.cpp epee/src/hex.cpp epee/src/string_tools.cpp epee/src/net_utils_base.cpp peer.cpp out_peer_factory.cpp inc_peer_factory.cpp CMakeLists.txt main.cpp)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(p2pnet 
	Threads::Threads
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_LIBRARIES}
)

install(TARGETS p2pnet RUNTIME DESTINATION bin)

add_subdirectory(p2p)
add_subdirectory(epee)
add_subdirectory(serialization)

add_subdirectory(cryptonote_basic)
add_subdirectory(common)