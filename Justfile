# Set shell for Windows
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

# Generator used for project
generator := "Visual Studio 17 2022"
# Where the project gets generated at
build := "build"
# Where the executable gets compiled to
bin := "bin"
# Where the library gets compiled to
libs := "libs"

lib_name := "velecs-ecs"

# Default target to show available commands
@default: help

# Show available commands
@help:
    just --list

@_setup-solution:
    if (!(Test-Path {{build}})) { New-Item -ItemType Directory -Path {{build}} -Force }
    cmake -S . -B {{build}} -G "{{generator}}"

# Build the library in debug mode
build: _setup-solution
    @echo "Building {{lib_name}} library (debug)..."
    cmake --build {{build}} --config Debug

# Build the library in release mode
build-release: _setup-solution
    @echo "Building {{lib_name}} library (release)..."
    cmake --build {{build}} --config Release

# Run tests
test: build
    @echo "Running tests..."
    ctest --test-dir {{build}} --output-on-failure -C Debug

# Run tests in release mode
test-release: build-release
    @echo "Running tests (release)..."
    ctest --test-dir {{build}} --output-on-failure -C Release

# Clean build directories
@clean:
    echo "Cleaning build directories..."
    if (Test-Path {{build}}) { Remove-Item -Recurse -Force {{build}} }

# Clean everything
@clean-all: clean
    echo "Cleaning everything..."
    if (Test-Path {{bin}}) { Remove-Item -Recurse -Force {{bin}} }