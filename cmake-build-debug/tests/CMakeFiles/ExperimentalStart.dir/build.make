# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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

# Utility rule file for ExperimentalStart.

# Include any custom commands dependencies for this target.
include tests/CMakeFiles/ExperimentalStart.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/ExperimentalStart.dir/progress.make

tests/CMakeFiles/ExperimentalStart:
	cd "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/tests" && /usr/bin/ctest -D ExperimentalStart

ExperimentalStart: tests/CMakeFiles/ExperimentalStart
ExperimentalStart: tests/CMakeFiles/ExperimentalStart.dir/build.make
.PHONY : ExperimentalStart

# Rule to build all files generated by this target.
tests/CMakeFiles/ExperimentalStart.dir/build: ExperimentalStart
.PHONY : tests/CMakeFiles/ExperimentalStart.dir/build

tests/CMakeFiles/ExperimentalStart.dir/clean:
	cd "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/tests" && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalStart.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/ExperimentalStart.dir/clean

tests/CMakeFiles/ExperimentalStart.dir/depend:
	cd "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/tests" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/tests" "/mnt/c/Users/illus/OneDrive - Technion/Semester 5/Operating Systems/HW4/wet4/cmake-build-debug/tests/CMakeFiles/ExperimentalStart.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/CMakeFiles/ExperimentalStart.dir/depend

