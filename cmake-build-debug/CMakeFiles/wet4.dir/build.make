# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/wet4.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/wet4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wet4.dir/flags.make

CMakeFiles/wet4.dir/main.cpp.o: CMakeFiles/wet4.dir/flags.make
CMakeFiles/wet4.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wet4.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wet4.dir/main.cpp.o -c "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/main.cpp"

CMakeFiles/wet4.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wet4.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/main.cpp" > CMakeFiles/wet4.dir/main.cpp.i

CMakeFiles/wet4.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wet4.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/main.cpp" -o CMakeFiles/wet4.dir/main.cpp.s

# Object files for target wet4
wet4_OBJECTS = \
"CMakeFiles/wet4.dir/main.cpp.o"

# External object files for target wet4
wet4_EXTERNAL_OBJECTS =

wet4: CMakeFiles/wet4.dir/main.cpp.o
wet4: CMakeFiles/wet4.dir/build.make
wet4: CMakeFiles/wet4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable wet4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wet4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wet4.dir/build: wet4
.PHONY : CMakeFiles/wet4.dir/build

CMakeFiles/wet4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wet4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wet4.dir/clean

CMakeFiles/wet4.dir/depend:
	cd "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/CMakeFiles/wet4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/wet4.dir/depend

