# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.3.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.3.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Workspace\GitHub\PL0-Compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/PL0_Compiler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PL0_Compiler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PL0_Compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PL0_Compiler.dir/flags.make

CMakeFiles/PL0_Compiler.dir/main.cpp.obj: CMakeFiles/PL0_Compiler.dir/flags.make
CMakeFiles/PL0_Compiler.dir/main.cpp.obj: ../main.cpp
CMakeFiles/PL0_Compiler.dir/main.cpp.obj: CMakeFiles/PL0_Compiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PL0_Compiler.dir/main.cpp.obj"
	D:\Programs\Environment\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PL0_Compiler.dir/main.cpp.obj -MF CMakeFiles\PL0_Compiler.dir\main.cpp.obj.d -o CMakeFiles\PL0_Compiler.dir\main.cpp.obj -c D:\Workspace\GitHub\PL0-Compiler\main.cpp

CMakeFiles/PL0_Compiler.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PL0_Compiler.dir/main.cpp.i"
	D:\Programs\Environment\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Workspace\GitHub\PL0-Compiler\main.cpp > CMakeFiles\PL0_Compiler.dir\main.cpp.i

CMakeFiles/PL0_Compiler.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PL0_Compiler.dir/main.cpp.s"
	D:\Programs\Environment\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Workspace\GitHub\PL0-Compiler\main.cpp -o CMakeFiles\PL0_Compiler.dir\main.cpp.s

# Object files for target PL0_Compiler
PL0_Compiler_OBJECTS = \
"CMakeFiles/PL0_Compiler.dir/main.cpp.obj"

# External object files for target PL0_Compiler
PL0_Compiler_EXTERNAL_OBJECTS =

PL0_Compiler.exe: CMakeFiles/PL0_Compiler.dir/main.cpp.obj
PL0_Compiler.exe: CMakeFiles/PL0_Compiler.dir/build.make
PL0_Compiler.exe: CMakeFiles/PL0_Compiler.dir/linklibs.rsp
PL0_Compiler.exe: CMakeFiles/PL0_Compiler.dir/objects1.rsp
PL0_Compiler.exe: CMakeFiles/PL0_Compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PL0_Compiler.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\PL0_Compiler.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PL0_Compiler.dir/build: PL0_Compiler.exe
.PHONY : CMakeFiles/PL0_Compiler.dir/build

CMakeFiles/PL0_Compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\PL0_Compiler.dir\cmake_clean.cmake
.PHONY : CMakeFiles/PL0_Compiler.dir/clean

CMakeFiles/PL0_Compiler.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Workspace\GitHub\PL0-Compiler D:\Workspace\GitHub\PL0-Compiler D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug D:\Workspace\GitHub\PL0-Compiler\cmake-build-debug\CMakeFiles\PL0_Compiler.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PL0_Compiler.dir/depend

