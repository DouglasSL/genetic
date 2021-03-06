# We require a sufficiently new version of CMake (for wxWidgets detection)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name
PROJECT(Evolve)

# Project settings
SET(WITH_OPENMP false)

# Enable warnings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Enable C++0x
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# Handle testing (tests are specified in "tests" subfolder")
ENABLE_TESTING()

# Add an "profile" build type
SET(CMAKE_CXX_FLAGS_PROFILE "-pg" CACHE STRING
    "Flags used by the C++ compiler during profiled builds."
    FORCE)
SET(CMAKE_C_FLAGS_PROFILE "-pg" CACHE STRING
    "Flags used by the C compiler during profiled builds."
    FORCE)
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_PROFILE
    CMAKE_C_FLAGS_PROFILE)

# Type of build ("debug", "release" or "profile")
set(CMAKE_BUILD_TYPE debug)

# Use OpenMP?
INCLUDE(CheckCCompilerFlag)
IF (WITH_OPENMP)
        CHECK_C_COMPILER_FLAG(-fopenmp HAVE_OPENMP)
        IF (HAVE_OPENMP)
                MESSAGE("** Using OpenMP multithreading")
                ADD_DEFINITIONS(-DWITH_OPENMP)
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -openmp")
                SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
        ELSE (HAVE_OPENMP)
                MESSAGE("!! Compiler doesn't support OpenMP multithreading, disabling multithreading")
        ENDIF (HAVE_OPENMP)
ENDIF (WITH_OPENMP)

# Add individual subdirectories
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(benchmarks)
