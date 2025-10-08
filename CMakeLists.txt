cmake_minimum_required(VERSION 3.14)
project(velecs-ecs VERSION 0.1.0)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Only fetch cmake utils when standalone
if(PROJECT_IS_TOP_LEVEL)
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
endif()

# Set output directories to bin if its the cmake build source
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT TARGET velecs-common)
    add_subdirectory(../velecs-common ${CMAKE_BINARY_DIR}/velecs-common)
endif()

if(NOT TARGET velecs-math)
    add_subdirectory(../velecs-math ${CMAKE_BINARY_DIR}/velecs-math)
endif()

add_subdirectory(libs/entt)

# Source files for the library
set(LIB_SOURCES
    # World
    src/World.cpp

    # Object
    src/Object.cpp

    # Scene
    src/SceneManager.cpp
    src/Scene.cpp
    
    # Entity
    src/Entity.cpp
    src/EntityBuilder.cpp

    # Tag
    src/Tag.cpp
    
    # Component
    src/Component.cpp
    src/Components/Transform.cpp

    # System
    src/System.cpp
)

# Header files for the library (for IDE organization)
set(LIB_HEADERS
    include/velecs/ecs/Common.hpp
    include/velecs/ecs/TypeConstraints.hpp

    # World
    include/velecs/ecs/World.hpp
    include/velecs/ecs/World.inl

    # Object
    include/velecs/ecs/Object.hpp
    include/velecs/ecs/Object.inl
    
    # Scene
    include/velecs/ecs/SceneManager.hpp
    include/velecs/ecs/Scene.hpp
    include/velecs/ecs/Scene.inl

    # Entity
    include/velecs/ecs/Entity.hpp
    include/velecs/ecs/Entity.inl
    include/velecs/ecs/EntityBuilder.hpp

    # Tag
    include/velecs/ecs/Tag.hpp
    include/velecs/ecs/Tags/DestroyTag.hpp

    # Component
    include/velecs/ecs/Component.hpp
    include/velecs/ecs/Components/Transform.hpp

    # System
    include/velecs/ecs/System.hpp
)

# Build the library
add_library(velecs-ecs ${LIB_SOURCES} ${LIB_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(velecs-ecs
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(velecs-ecs
    PUBLIC velecs-common
    PUBLIC velecs-math
    PUBLIC EnTT::EnTT
)

# Fetch Google Test (will reuse if already fetched by parent)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    FIND_PACKAGE_ARGS NAMES GTest
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Create test executable
add_executable(velecs-ecs-tests
    tests/test.cpp
    # Add more test files here
)

target_link_libraries(velecs-ecs-tests
    PRIVATE velecs-ecs
    PRIVATE GTest::gtest_main
    PRIVATE GTest::gmock
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(velecs-ecs-tests)

# Copy ASan DLLs if needed
if(PROJECT_IS_TOP_LEVEL)
    copy_asan_dlls(velecs-ecs-tests)
endif()

# Export the library target for use by parent projects
if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # We're being included as a submodule
    set(VELECS_ECS_LIBRARIES velecs-ecs PARENT_SCOPE)
endif()
