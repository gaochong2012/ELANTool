# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/share/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/share/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gaochong/GC_DDD/ELANTool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gaochong/GC_DDD/ELANTool/cmake-build-debug

# Utility rule file for ELANTool_autogen.

# Include the progress variables for this target.
include CMakeFiles/ELANTool_autogen.dir/progress.make

CMakeFiles/ELANTool_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/gaochong/GC_DDD/ELANTool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target ELANTool"
	/usr/share/clion/bin/cmake/bin/cmake -E cmake_autogen /home/gaochong/GC_DDD/ELANTool/cmake-build-debug/CMakeFiles/ELANTool_autogen.dir/ Debug

ELANTool_autogen: CMakeFiles/ELANTool_autogen
ELANTool_autogen: CMakeFiles/ELANTool_autogen.dir/build.make

.PHONY : ELANTool_autogen

# Rule to build all files generated by this target.
CMakeFiles/ELANTool_autogen.dir/build: ELANTool_autogen

.PHONY : CMakeFiles/ELANTool_autogen.dir/build

CMakeFiles/ELANTool_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ELANTool_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ELANTool_autogen.dir/clean

CMakeFiles/ELANTool_autogen.dir/depend:
	cd /home/gaochong/GC_DDD/ELANTool/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gaochong/GC_DDD/ELANTool /home/gaochong/GC_DDD/ELANTool /home/gaochong/GC_DDD/ELANTool/cmake-build-debug /home/gaochong/GC_DDD/ELANTool/cmake-build-debug /home/gaochong/GC_DDD/ELANTool/cmake-build-debug/CMakeFiles/ELANTool_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ELANTool_autogen.dir/depend

