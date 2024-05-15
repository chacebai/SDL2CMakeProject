# CMakeList.txt : CMake project for SDL2CMakeProject, include source and define
# project specific logic here.
#

FILE(GLOB_RECURSE all_source_files "src/*.cpp" "src/*.c")
FILE(GLOB_RECURSE all_head_files "src/*.hpp" "src*.h")

# Add source to this project's executable.
add_executable (SDL2CMakeProject "SDL2CMakeProject.cpp" "SDL2CMakeProject.h" ${all_head_files} ${all_source_files})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SDL2CMakeProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

#include_directories(${PROJECT_SOURCE_DIR}/SDL2/include)
#include_directories(${PROJECT_SOURCE_DIR}/SDL2/include/SDL2)

# Link Windows socket libraries
#target_link_libraries(SDL2CMakeProject ws2_32.lib gdi32.lib advapi32.lib crypt32.lib winmm.lib)
#target_link_libraries(SDL2CMakeProject ${PROJECT_SOURCE_DIR}/SDL2/lib/SDL2d.dll)
#target_link_libraries(SDL2CMakeProject ${PROJECT_SOURCE_DIR}/SDL2/lib/SDL2maind.lib)

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(SDL2CMakeProject PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(SDL2CMakeProject PRIVATE SDL2::SDL2-static)
