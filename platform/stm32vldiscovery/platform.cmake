set(DEVICE "stm32f100rbt6")
set(FREERTOS_PORT "GCC_ARM_CM3" CACHE STRING "" FORCE)
set(LIBOPENCM3_TARGET "stm32/f1")
set(ENV{SVD_FILE} "STM32F100.svd")
set(ENV{OPENOCD_CONFIG} "/usr/share/openocd/scripts/board/stm32vldiscovery.cfg")

set(PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/platform/${PLATFORM}/platform.c
)

add_link_options(
    -T ${CMAKE_SOURCE_DIR}/platform/${PLATFORM}/platform.ld
)

add_compile_definitions(
    -DSTM32F1
)