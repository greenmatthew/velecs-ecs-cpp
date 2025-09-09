# Set shell for Windows
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

# Configuration
generator := "Visual Studio 17 2022"  # Visual Studio generator
build := "build"                      # CMake build directory
bin := "bin"                          # Binary output directory
exe := "velecs-ecs-test.exe"          # Test executable name

# Default target - show available commands
default: help

# Show all available commands
@help:
    just --list

# Generate Visual Studio solution and install dependencies
@build-solution:
    echo "Generating Visual Studio solution..."
    if (!(Test-Path {{build}})) { New-Item -ItemType Directory -Path {{build}} -Force > $null }
    cmake -S . -B {{build}} -DVELECS_ECS_BUILD_TESTS=ON -G "{{generator}}" -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

# Build the project in debug mode
@build: build-solution
    echo "Building project (Debug)..."
    cmake --build {{build}} --config Debug

# Build the project in release mode  
@build-release: build-solution
    echo "Building project (Release)..."
    cmake --build {{build}} --config Release

# Build and run the test executable (debug)
@run: build
    echo "Running test executable (Debug)..."
    {{bin}}/Debug/{{exe}}

# Build and run the test executable (release)
@run-release: build-release
    echo "Running test executable (Release)..."
    {{bin}}/Release/{{exe}}

# Clean build directory
@clean:
    if (Test-Path {{build}}) { Remove-Item -Recurse -Force {{build}}; echo "Cleaned build directory..." }
