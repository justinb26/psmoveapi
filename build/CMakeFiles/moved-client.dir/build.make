# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/justinb/Developer/psmoveapi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/justinb/Developer/psmoveapi/build

# Include any dependencies generated for this target.
include CMakeFiles/moved-client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/moved-client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/moved-client.dir/flags.make

CMakeFiles/moved-client.dir/moved_client_test.c.o: CMakeFiles/moved-client.dir/flags.make
CMakeFiles/moved-client.dir/moved_client_test.c.o: ../moved_client_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/justinb/Developer/psmoveapi/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/moved-client.dir/moved_client_test.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/moved-client.dir/moved_client_test.c.o   -c /Users/justinb/Developer/psmoveapi/moved_client_test.c

CMakeFiles/moved-client.dir/moved_client_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/moved-client.dir/moved_client_test.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /Users/justinb/Developer/psmoveapi/moved_client_test.c > CMakeFiles/moved-client.dir/moved_client_test.c.i

CMakeFiles/moved-client.dir/moved_client_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/moved-client.dir/moved_client_test.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /Users/justinb/Developer/psmoveapi/moved_client_test.c -o CMakeFiles/moved-client.dir/moved_client_test.c.s

CMakeFiles/moved-client.dir/moved_client_test.c.o.requires:
.PHONY : CMakeFiles/moved-client.dir/moved_client_test.c.o.requires

CMakeFiles/moved-client.dir/moved_client_test.c.o.provides: CMakeFiles/moved-client.dir/moved_client_test.c.o.requires
	$(MAKE) -f CMakeFiles/moved-client.dir/build.make CMakeFiles/moved-client.dir/moved_client_test.c.o.provides.build
.PHONY : CMakeFiles/moved-client.dir/moved_client_test.c.o.provides

CMakeFiles/moved-client.dir/moved_client_test.c.o.provides.build: CMakeFiles/moved-client.dir/moved_client_test.c.o

CMakeFiles/moved-client.dir/moved_client.c.o: CMakeFiles/moved-client.dir/flags.make
CMakeFiles/moved-client.dir/moved_client.c.o: ../moved_client.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/justinb/Developer/psmoveapi/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/moved-client.dir/moved_client.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/moved-client.dir/moved_client.c.o   -c /Users/justinb/Developer/psmoveapi/moved_client.c

CMakeFiles/moved-client.dir/moved_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/moved-client.dir/moved_client.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /Users/justinb/Developer/psmoveapi/moved_client.c > CMakeFiles/moved-client.dir/moved_client.c.i

CMakeFiles/moved-client.dir/moved_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/moved-client.dir/moved_client.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /Users/justinb/Developer/psmoveapi/moved_client.c -o CMakeFiles/moved-client.dir/moved_client.c.s

CMakeFiles/moved-client.dir/moved_client.c.o.requires:
.PHONY : CMakeFiles/moved-client.dir/moved_client.c.o.requires

CMakeFiles/moved-client.dir/moved_client.c.o.provides: CMakeFiles/moved-client.dir/moved_client.c.o.requires
	$(MAKE) -f CMakeFiles/moved-client.dir/build.make CMakeFiles/moved-client.dir/moved_client.c.o.provides.build
.PHONY : CMakeFiles/moved-client.dir/moved_client.c.o.provides

CMakeFiles/moved-client.dir/moved_client.c.o.provides.build: CMakeFiles/moved-client.dir/moved_client.c.o

# Object files for target moved-client
moved__client_OBJECTS = \
"CMakeFiles/moved-client.dir/moved_client_test.c.o" \
"CMakeFiles/moved-client.dir/moved_client.c.o"

# External object files for target moved-client
moved__client_EXTERNAL_OBJECTS =

moved-client: CMakeFiles/moved-client.dir/moved_client_test.c.o
moved-client: CMakeFiles/moved-client.dir/moved_client.c.o
moved-client: CMakeFiles/moved-client.dir/build.make
moved-client: CMakeFiles/moved-client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable moved-client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/moved-client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/moved-client.dir/build: moved-client
.PHONY : CMakeFiles/moved-client.dir/build

CMakeFiles/moved-client.dir/requires: CMakeFiles/moved-client.dir/moved_client_test.c.o.requires
CMakeFiles/moved-client.dir/requires: CMakeFiles/moved-client.dir/moved_client.c.o.requires
.PHONY : CMakeFiles/moved-client.dir/requires

CMakeFiles/moved-client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/moved-client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/moved-client.dir/clean

CMakeFiles/moved-client.dir/depend:
	cd /Users/justinb/Developer/psmoveapi/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/justinb/Developer/psmoveapi /Users/justinb/Developer/psmoveapi /Users/justinb/Developer/psmoveapi/build /Users/justinb/Developer/psmoveapi/build /Users/justinb/Developer/psmoveapi/build/CMakeFiles/moved-client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/moved-client.dir/depend

