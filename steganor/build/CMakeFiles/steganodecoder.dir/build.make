# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/jjoska/C/C/steganor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jjoska/C/C/steganor/build

# Include any dependencies generated for this target.
include CMakeFiles/steganodecoder.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/steganodecoder.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/steganodecoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/steganodecoder.dir/flags.make

CMakeFiles/steganodecoder.dir/src/decoder.c.o: CMakeFiles/steganodecoder.dir/flags.make
CMakeFiles/steganodecoder.dir/src/decoder.c.o: /home/jjoska/C/C/steganor/src/decoder.c
CMakeFiles/steganodecoder.dir/src/decoder.c.o: CMakeFiles/steganodecoder.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jjoska/C/C/steganor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/steganodecoder.dir/src/decoder.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/steganodecoder.dir/src/decoder.c.o -MF CMakeFiles/steganodecoder.dir/src/decoder.c.o.d -o CMakeFiles/steganodecoder.dir/src/decoder.c.o -c /home/jjoska/C/C/steganor/src/decoder.c

CMakeFiles/steganodecoder.dir/src/decoder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/steganodecoder.dir/src/decoder.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jjoska/C/C/steganor/src/decoder.c > CMakeFiles/steganodecoder.dir/src/decoder.c.i

CMakeFiles/steganodecoder.dir/src/decoder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/steganodecoder.dir/src/decoder.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jjoska/C/C/steganor/src/decoder.c -o CMakeFiles/steganodecoder.dir/src/decoder.c.s

CMakeFiles/steganodecoder.dir/src/imgio.c.o: CMakeFiles/steganodecoder.dir/flags.make
CMakeFiles/steganodecoder.dir/src/imgio.c.o: /home/jjoska/C/C/steganor/src/imgio.c
CMakeFiles/steganodecoder.dir/src/imgio.c.o: CMakeFiles/steganodecoder.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jjoska/C/C/steganor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/steganodecoder.dir/src/imgio.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/steganodecoder.dir/src/imgio.c.o -MF CMakeFiles/steganodecoder.dir/src/imgio.c.o.d -o CMakeFiles/steganodecoder.dir/src/imgio.c.o -c /home/jjoska/C/C/steganor/src/imgio.c

CMakeFiles/steganodecoder.dir/src/imgio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/steganodecoder.dir/src/imgio.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jjoska/C/C/steganor/src/imgio.c > CMakeFiles/steganodecoder.dir/src/imgio.c.i

CMakeFiles/steganodecoder.dir/src/imgio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/steganodecoder.dir/src/imgio.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jjoska/C/C/steganor/src/imgio.c -o CMakeFiles/steganodecoder.dir/src/imgio.c.s

# Object files for target steganodecoder
steganodecoder_OBJECTS = \
"CMakeFiles/steganodecoder.dir/src/decoder.c.o" \
"CMakeFiles/steganodecoder.dir/src/imgio.c.o"

# External object files for target steganodecoder
steganodecoder_EXTERNAL_OBJECTS =

steganodecoder: CMakeFiles/steganodecoder.dir/src/decoder.c.o
steganodecoder: CMakeFiles/steganodecoder.dir/src/imgio.c.o
steganodecoder: CMakeFiles/steganodecoder.dir/build.make
steganodecoder: CMakeFiles/steganodecoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jjoska/C/C/steganor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable steganodecoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/steganodecoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/steganodecoder.dir/build: steganodecoder
.PHONY : CMakeFiles/steganodecoder.dir/build

CMakeFiles/steganodecoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/steganodecoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/steganodecoder.dir/clean

CMakeFiles/steganodecoder.dir/depend:
	cd /home/jjoska/C/C/steganor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jjoska/C/C/steganor /home/jjoska/C/C/steganor /home/jjoska/C/C/steganor/build /home/jjoska/C/C/steganor/build /home/jjoska/C/C/steganor/build/CMakeFiles/steganodecoder.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/steganodecoder.dir/depend

