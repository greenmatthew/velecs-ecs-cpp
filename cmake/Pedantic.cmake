cmake_minimum_required(VERSION 3.14)

option(ENABLE_PEDANTIC "Enable pedantic compiler warnings" OFF)

set(PEDANTIC_CONDITION "$<BOOL:${ENABLE_PEDANTIC}>")

if (MSVC)
  set(PEDANTIC_FLAGS
    "$<${PEDANTIC_CONDITION}:/W4>"              # Warning level 4
    "$<${PEDANTIC_CONDITION}:/permissive->"     # Strict standards conformance
  )
endif()

add_compile_options(${PEDANTIC_FLAGS})