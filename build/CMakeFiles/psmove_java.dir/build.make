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
include CMakeFiles/psmove_java.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/psmove_java.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/psmove_java.dir/flags.make

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o: CMakeFiles/psmove_java.dir/flags.make
CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o: psmoveJAVA_wrap.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/justinb/Developer/psmoveapi/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o   -c /Users/justinb/Developer/psmoveapi/build/psmoveJAVA_wrap.c

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /Users/justinb/Developer/psmoveapi/build/psmoveJAVA_wrap.c > CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.i

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /Users/justinb/Developer/psmoveapi/build/psmoveJAVA_wrap.c -o CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.s

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.requires:
.PHONY : CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.requires

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.provides: CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.requires
	$(MAKE) -f CMakeFiles/psmove_java.dir/build.make CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.provides.build
.PHONY : CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.provides

CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.provides.build: CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o

psmoveJAVA_wrap.c: ../psmove.i
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/justinb/Developer/psmoveapi/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Swig source"
	/opt/local/bin/cmake -E make_directory /Users/justinb/Developer/psmoveapi/build
	/opt/local/bin/swig -java -package io.thp.psmove -outdir /Users/justinb/Developer/psmoveapi/build -I/Users/justinb/Developer/psmoveapi/hidapi/hidapi -I/Users/justinb/Developer/psmoveapi -I/System/Library/Frameworks/Python.framework/Headers -I/System/Library/Frameworks/JavaVM.framework/Headers -o /Users/justinb/Developer/psmoveapi/build/psmoveJAVA_wrap.c /Users/justinb/Developer/psmoveapi/psmove.i

# Object files for target psmove_java
psmove_java_OBJECTS = \
"CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o"

# External object files for target psmove_java
psmove_java_EXTERNAL_OBJECTS =

libpsmove_java.jnilib: CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o
libpsmove_java.jnilib: libpsmoveapi.1.0.2.dylib
libpsmove_java.jnilib: CMakeFiles/psmove_java.dir/build.make
libpsmove_java.jnilib: CMakeFiles/psmove_java.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module libpsmove_java.jnilib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/psmove_java.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/psmove_java.dir/build: libpsmove_java.jnilib
.PHONY : CMakeFiles/psmove_java.dir/build

CMakeFiles/psmove_java.dir/requires: CMakeFiles/psmove_java.dir/psmoveJAVA_wrap.c.o.requires
.PHONY : CMakeFiles/psmove_java.dir/requires

CMakeFiles/psmove_java.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/psmove_java.dir/cmake_clean.cmake
.PHONY : CMakeFiles/psmove_java.dir/clean

CMakeFiles/psmove_java.dir/depend: psmoveJAVA_wrap.c
	cd /Users/justinb/Developer/psmoveapi/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/justinb/Developer/psmoveapi /Users/justinb/Developer/psmoveapi /Users/justinb/Developer/psmoveapi/build /Users/justinb/Developer/psmoveapi/build /Users/justinb/Developer/psmoveapi/build/CMakeFiles/psmove_java.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/psmove_java.dir/depend

