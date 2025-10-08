cmake_minimum_required(VERSION 3.14)
project(velecs-ecs-sandbox)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Add the velecs-ecs library (parent-parent directory since we're in tests/Sandbox)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_BINARY_DIR}/velecs-ecs)

# Create sandbox executable
add_executable(velecs-ecs-sandbox sandbox.cpp)

# Link against velecs-ecs
target_link_libraries(velecs-ecs-sandbox PRIVATE velecs-ecs)

# Set as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT velecs-ecs-sandbox)
