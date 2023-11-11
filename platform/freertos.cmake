
FetchContent_Declare( 
  freertos_kernel
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
  GIT_TAG        V10.6.1
)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
    INTERFACE
        ${CMAKE_SOURCE_DIR}/platform/${PLATFORM}
)

target_compile_definitions(freertos_config
    INTERFACE
        projCOVERAGE_TEST=0
)

FetchContent_MakeAvailable(freertos_kernel)


