cmake_minimum_required(VERSION 3.23)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

set(SANITIZE_CONDITION "$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_ASAN}>>")

if (MSVC)
  set(
    SANITIZE_FLAGS 
    "$<${SANITIZE_CONDITION}:/fsanitize=address>"
  )
  add_compile_options("$<${SANITIZE_CONDITION}:/Zi>")
endif()

add_compile_options(${SANITIZE_FLAGS})
add_link_options(${SANITIZE_FLAGS})

# Helper function to copy ASan DLLs for a target
function(copy_asan_dlls TARGET_NAME)
  if(MSVC AND ENABLE_ASAN)
    # Find the ASan runtime DLLs relative to the compiler
    get_filename_component(COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(ASAN_DLL_DBG "${COMPILER_DIR}/clang_rt.asan_dbg_dynamic-x86_64.dll")
    set(ASAN_DLL_REL "${COMPILER_DIR}/clang_rt.asan_dynamic-x86_64.dll")
    
    # Copy debug DLL if it exists
    if(EXISTS ${ASAN_DLL_DBG})
      add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${ASAN_DLL_DBG}
          $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMENT "Copying ASan debug DLL for ${TARGET_NAME}"
      )
      message(STATUS "Will copy ASan debug DLL for ${TARGET_NAME}: ${ASAN_DLL_DBG}")
    endif()
    
    # Copy release DLL if it exists
    if(EXISTS ${ASAN_DLL_REL})
      add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${ASAN_DLL_REL}
          $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMENT "Copying ASan release DLL for ${TARGET_NAME}"
      )
      message(STATUS "Will copy ASan release DLL for ${TARGET_NAME}: ${ASAN_DLL_REL}")
    endif()
  endif()
endfunction()
