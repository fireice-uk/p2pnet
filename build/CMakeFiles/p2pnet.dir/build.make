# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/benqgg/projects/p2pnet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/benqgg/projects/p2pnet/build

# Include any dependencies generated for this target.
include CMakeFiles/p2pnet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/p2pnet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/p2pnet.dir/flags.make

CMakeFiles/p2pnet.dir/smart_socket.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/smart_socket.cpp.o: ../smart_socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/p2pnet.dir/smart_socket.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/smart_socket.cpp.o -c /home/benqgg/projects/p2pnet/smart_socket.cpp

CMakeFiles/p2pnet.dir/smart_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/smart_socket.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/smart_socket.cpp > CMakeFiles/p2pnet.dir/smart_socket.cpp.i

CMakeFiles/p2pnet.dir/smart_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/smart_socket.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/smart_socket.cpp -o CMakeFiles/p2pnet.dir/smart_socket.cpp.s

CMakeFiles/p2pnet.dir/smart_socket.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/smart_socket.cpp.o.requires

CMakeFiles/p2pnet.dir/smart_socket.cpp.o.provides: CMakeFiles/p2pnet.dir/smart_socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/smart_socket.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/smart_socket.cpp.o.provides

CMakeFiles/p2pnet.dir/smart_socket.cpp.o.provides.build: CMakeFiles/p2pnet.dir/smart_socket.cpp.o


CMakeFiles/p2pnet.dir/peer.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/peer.cpp.o: ../peer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/p2pnet.dir/peer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/peer.cpp.o -c /home/benqgg/projects/p2pnet/peer.cpp

CMakeFiles/p2pnet.dir/peer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/peer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/peer.cpp > CMakeFiles/p2pnet.dir/peer.cpp.i

CMakeFiles/p2pnet.dir/peer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/peer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/peer.cpp -o CMakeFiles/p2pnet.dir/peer.cpp.s

CMakeFiles/p2pnet.dir/peer.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/peer.cpp.o.requires

CMakeFiles/p2pnet.dir/peer.cpp.o.provides: CMakeFiles/p2pnet.dir/peer.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/peer.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/peer.cpp.o.provides

CMakeFiles/p2pnet.dir/peer.cpp.o.provides.build: CMakeFiles/p2pnet.dir/peer.cpp.o


CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o: ../out_peer_factory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o -c /home/benqgg/projects/p2pnet/out_peer_factory.cpp

CMakeFiles/p2pnet.dir/out_peer_factory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/out_peer_factory.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/out_peer_factory.cpp > CMakeFiles/p2pnet.dir/out_peer_factory.cpp.i

CMakeFiles/p2pnet.dir/out_peer_factory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/out_peer_factory.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/out_peer_factory.cpp -o CMakeFiles/p2pnet.dir/out_peer_factory.cpp.s

CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.requires

CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.provides: CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.provides

CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.provides.build: CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o


CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o: ../inc_peer_factory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o -c /home/benqgg/projects/p2pnet/inc_peer_factory.cpp

CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/inc_peer_factory.cpp > CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.i

CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/inc_peer_factory.cpp -o CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.s

CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.requires

CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.provides: CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.provides

CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.provides.build: CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o


CMakeFiles/p2pnet.dir/adress_container.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/adress_container.cpp.o: ../adress_container.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/p2pnet.dir/adress_container.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/adress_container.cpp.o -c /home/benqgg/projects/p2pnet/adress_container.cpp

CMakeFiles/p2pnet.dir/adress_container.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/adress_container.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/adress_container.cpp > CMakeFiles/p2pnet.dir/adress_container.cpp.i

CMakeFiles/p2pnet.dir/adress_container.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/adress_container.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/adress_container.cpp -o CMakeFiles/p2pnet.dir/adress_container.cpp.s

CMakeFiles/p2pnet.dir/adress_container.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/adress_container.cpp.o.requires

CMakeFiles/p2pnet.dir/adress_container.cpp.o.provides: CMakeFiles/p2pnet.dir/adress_container.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/adress_container.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/adress_container.cpp.o.provides

CMakeFiles/p2pnet.dir/adress_container.cpp.o.provides.build: CMakeFiles/p2pnet.dir/adress_container.cpp.o


CMakeFiles/p2pnet.dir/main.cpp.o: CMakeFiles/p2pnet.dir/flags.make
CMakeFiles/p2pnet.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/p2pnet.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2pnet.dir/main.cpp.o -c /home/benqgg/projects/p2pnet/main.cpp

CMakeFiles/p2pnet.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2pnet.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/benqgg/projects/p2pnet/main.cpp > CMakeFiles/p2pnet.dir/main.cpp.i

CMakeFiles/p2pnet.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2pnet.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/benqgg/projects/p2pnet/main.cpp -o CMakeFiles/p2pnet.dir/main.cpp.s

CMakeFiles/p2pnet.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/p2pnet.dir/main.cpp.o.requires

CMakeFiles/p2pnet.dir/main.cpp.o.provides: CMakeFiles/p2pnet.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2pnet.dir/build.make CMakeFiles/p2pnet.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/p2pnet.dir/main.cpp.o.provides

CMakeFiles/p2pnet.dir/main.cpp.o.provides.build: CMakeFiles/p2pnet.dir/main.cpp.o


# Object files for target p2pnet
p2pnet_OBJECTS = \
"CMakeFiles/p2pnet.dir/smart_socket.cpp.o" \
"CMakeFiles/p2pnet.dir/peer.cpp.o" \
"CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o" \
"CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o" \
"CMakeFiles/p2pnet.dir/adress_container.cpp.o" \
"CMakeFiles/p2pnet.dir/main.cpp.o"

# External object files for target p2pnet
p2pnet_EXTERNAL_OBJECTS =

p2pnet: CMakeFiles/p2pnet.dir/smart_socket.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/peer.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/adress_container.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/main.cpp.o
p2pnet: CMakeFiles/p2pnet.dir/build.make
p2pnet: CMakeFiles/p2pnet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/benqgg/projects/p2pnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable p2pnet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/p2pnet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/p2pnet.dir/build: p2pnet

.PHONY : CMakeFiles/p2pnet.dir/build

CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/smart_socket.cpp.o.requires
CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/peer.cpp.o.requires
CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/out_peer_factory.cpp.o.requires
CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/inc_peer_factory.cpp.o.requires
CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/adress_container.cpp.o.requires
CMakeFiles/p2pnet.dir/requires: CMakeFiles/p2pnet.dir/main.cpp.o.requires

.PHONY : CMakeFiles/p2pnet.dir/requires

CMakeFiles/p2pnet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/p2pnet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/p2pnet.dir/clean

CMakeFiles/p2pnet.dir/depend:
	cd /home/benqgg/projects/p2pnet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/benqgg/projects/p2pnet /home/benqgg/projects/p2pnet /home/benqgg/projects/p2pnet/build /home/benqgg/projects/p2pnet/build /home/benqgg/projects/p2pnet/build/CMakeFiles/p2pnet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/p2pnet.dir/depend

