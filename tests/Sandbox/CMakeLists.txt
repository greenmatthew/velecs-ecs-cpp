cmake_minimum_required(VERSION 3.14)
project(velecs-ecs-sandbox)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    velecs_cmake_utils
    GIT_REPOSITORY https://github.com/greenmatthew/velecs-cmake-utils.git
    GIT_TAG master
)
FetchContent_MakeAvailable(velecs_cmake_utils)

# Add to module path
list(APPEND CMAKE_MODULE_PATH ${velecs_cmake_utils_SOURCE_DIR}/cmake)

# Enable pedantic warnings
set(ENABLE_PEDANTIC ON CACHE BOOL "Enable pedantic compiler warnings")
include(compile/Pedantic)

# Enable AddressSanitizer
set(ENABLE_ASAN ON CACHE BOOL "Enable AddressSanitizer")
include(compile/ASan)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Add the velecs-ecs library (parent-parent directory since we're in tests/Sandbox)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_BINARY_DIR}/velecs-ecs)

# Create sandbox executable
add_executable(velecs-ecs-sandbox sandbox.cpp)

# Link against velecs-ecs
target_link_libraries(velecs-ecs-sandbox PRIVATE velecs-ecs)

copy_asan_dlls(velecs-ecs-sandbox)

# Set as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT velecs-ecs-sandbox)
