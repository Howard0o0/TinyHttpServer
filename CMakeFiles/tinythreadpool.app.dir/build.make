# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/howard/work/TinyThreadPool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/howard/work/TinyThreadPool

# Include any dependencies generated for this target.
include CMakeFiles/tinythreadpool.app.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tinythreadpool.app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tinythreadpool.app.dir/flags.make

CMakeFiles/tinythreadpool.app.dir/src/main.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/main.cc.o: src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/main.cc.o -c /home/howard/work/TinyThreadPool/src/main.cc

CMakeFiles/tinythreadpool.app.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/main.cc > CMakeFiles/tinythreadpool.app.dir/src/main.cc.i

CMakeFiles/tinythreadpool.app.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/main.cc -o CMakeFiles/tinythreadpool.app.dir/src/main.cc.s

CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/main.cc.o


CMakeFiles/tinythreadpool.app.dir/src/test.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/test.cc.o: src/test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/test.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/test.cc.o -c /home/howard/work/TinyThreadPool/src/test.cc

CMakeFiles/tinythreadpool.app.dir/src/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/test.cc > CMakeFiles/tinythreadpool.app.dir/src/test.cc.i

CMakeFiles/tinythreadpool.app.dir/src/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/test.cc -o CMakeFiles/tinythreadpool.app.dir/src/test.cc.s

CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/test.cc.o


CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o: src/base/lockfreethreadpool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o -c /home/howard/work/TinyThreadPool/src/base/lockfreethreadpool.cc

CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/base/lockfreethreadpool.cc > CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.i

CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/base/lockfreethreadpool.cc -o CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.s

CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o


CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o: src/base/thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o -c /home/howard/work/TinyThreadPool/src/base/thread.cc

CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/base/thread.cc > CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.i

CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/base/thread.cc -o CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.s

CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o


CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o: src/base/threadpool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o -c /home/howard/work/TinyThreadPool/src/base/threadpool.cc

CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/base/threadpool.cc > CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.i

CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/base/threadpool.cc -o CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.s

CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o


CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o: src/net/epolltool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o -c /home/howard/work/TinyThreadPool/src/net/epolltool.cc

CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/net/epolltool.cc > CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.i

CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/net/epolltool.cc -o CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.s

CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o


CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o: src/net/httpserver.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o -c /home/howard/work/TinyThreadPool/src/net/httpserver.cc

CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/net/httpserver.cc > CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.i

CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/net/httpserver.cc -o CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.s

CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o


CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o: src/net/sockettool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o -c /home/howard/work/TinyThreadPool/src/net/sockettool.cc

CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/net/sockettool.cc > CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.i

CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/net/sockettool.cc -o CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.s

CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o


CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o: src/net/tcpserver.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o -c /home/howard/work/TinyThreadPool/src/net/tcpserver.cc

CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/net/tcpserver.cc > CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.i

CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/net/tcpserver.cc -o CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.s

CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o


CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o: CMakeFiles/tinythreadpool.app.dir/flags.make
CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o: src/net/worker.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o -c /home/howard/work/TinyThreadPool/src/net/worker.cc

CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/howard/work/TinyThreadPool/src/net/worker.cc > CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.i

CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/howard/work/TinyThreadPool/src/net/worker.cc -o CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.s

CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.requires:

.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.requires

CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.provides: CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.requires
	$(MAKE) -f CMakeFiles/tinythreadpool.app.dir/build.make CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.provides.build
.PHONY : CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.provides

CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.provides.build: CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o


# Object files for target tinythreadpool.app
tinythreadpool_app_OBJECTS = \
"CMakeFiles/tinythreadpool.app.dir/src/main.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/test.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o" \
"CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o"

# External object files for target tinythreadpool.app
tinythreadpool_app_EXTERNAL_OBJECTS =

tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/main.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/test.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/build.make
tinythreadpool.app: third-party-libs/muduo/lib/libmuduo_net.a
tinythreadpool.app: third-party-libs/muduo/lib/libmuduo_base.a
tinythreadpool.app: CMakeFiles/tinythreadpool.app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/howard/work/TinyThreadPool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable tinythreadpool.app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinythreadpool.app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tinythreadpool.app.dir/build: tinythreadpool.app

.PHONY : CMakeFiles/tinythreadpool.app.dir/build

CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/main.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/test.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/base/lockfreethreadpool.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/base/thread.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/base/threadpool.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/net/epolltool.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/net/httpserver.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/net/sockettool.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/net/tcpserver.cc.o.requires
CMakeFiles/tinythreadpool.app.dir/requires: CMakeFiles/tinythreadpool.app.dir/src/net/worker.cc.o.requires

.PHONY : CMakeFiles/tinythreadpool.app.dir/requires

CMakeFiles/tinythreadpool.app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tinythreadpool.app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tinythreadpool.app.dir/clean

CMakeFiles/tinythreadpool.app.dir/depend:
	cd /home/howard/work/TinyThreadPool && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/howard/work/TinyThreadPool /home/howard/work/TinyThreadPool /home/howard/work/TinyThreadPool /home/howard/work/TinyThreadPool /home/howard/work/TinyThreadPool/CMakeFiles/tinythreadpool.app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tinythreadpool.app.dir/depend

