# Use ARM-GCC for crosscompiling
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PLATFORM "stm32vldiscovery")

find_program(ARM_CC arm-none-eabi-gcc)
find_program(ARM_CXX arm-none-eabi-g++)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_SIZE arm-none-eabi-size)

set(PLATFORM_CFLAGS
    -mcpu=cortex-m3
    -mfloat-abi=soft
    -mthumb
    --specs=nosys.specs
    -ffunction-sections
    -fdata-sections
    # -pic
)

set(PLATFORM_LDFLAGS
    -mcpu=cortex-m3
    -mfloat-abi=soft
    -mthumb
    -Wl,--gc-sections
    -mfloat-abi=soft
    --specs=nosys.specs
    -nostartfiles
    -static
)

add_compile_options(${PLATFORM_CFLAGS})
add_link_options(${PLATFORM_LDFLAGS})

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER ${ARM_CC})
set(CMAKE_CXX_COMPILER ${ARM_CXX})