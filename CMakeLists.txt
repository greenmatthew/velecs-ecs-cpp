cmake_minimum_required(VERSION 3.14)
project(velecs-ecs VERSION 0.1.0)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories to bin if its the cmake build source
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Only set output directories when building this library standalone
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to control whether to build the test executable
option(VELECS_ECS_BUILD_TESTS "Build test executable for velecs-ecs" OFF)

if(NOT TARGET velecs-common)
    add_subdirectory(../velecs-common ${CMAKE_BINARY_DIR}/velecs-common)
endif()

if(NOT TARGET velecs-math)
    add_subdirectory(../velecs-math ${CMAKE_BINARY_DIR}/velecs-math)
endif()

add_subdirectory(libs/entt)

# Source files for the library
set(LIB_SOURCES
    src/test/main.cpp

    # Objects
    src/World.cpp
    src/Object.cpp

    # Scenes
    src/SceneManager.cpp
    src/Scene.cpp
    

    # Entities
    src/Entity.cpp
    src/EntityBuilder.cpp


    # Tags
    src/Tag.cpp
    

    # Components
    src/Component.cpp
    src/Components/Transform.cpp

    # Systems
    src/System.cpp
)

# Header files for the library (for IDE organization)
set(LIB_HEADERS
    include/velecs/ecs/Common.hpp
    include/velecs/ecs/TypeConstraints.hpp

    # Objects
    include/velecs/ecs/World.hpp
    include/velecs/ecs/Object.hpp
    
    # Scenes
    include/velecs/ecs/SceneManager.hpp
    include/velecs/ecs/Scene.hpp

    # Entities
    include/velecs/ecs/Entity.hpp
    include/velecs/ecs/Entity.inl
    include/velecs/ecs/EntityBuilder.hpp

    # Tags
    include/velecs/ecs/Tag.hpp
    include/velecs/ecs/Tags/DestroyTag.hpp

    # Components
    include/velecs/ecs/Component.hpp
    include/velecs/ecs/Components/Transform.hpp

    # Systems
    include/velecs/ecs/System.hpp
)

# Always build the library
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

# Conditionally build the test executable
if(VELECS_ECS_BUILD_TESTS)
    add_executable(velecs-ecs-test src/test/main.cpp)
    target_link_libraries(velecs-ecs-test PRIVATE velecs-ecs)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT velecs-ecs-test)
endif()

# Export the library target for use by parent projects
if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # We're being included as a submodule
    set(VELECS_ECS_LIBRARIES velecs-ecs PARENT_SCOPE)
endif()