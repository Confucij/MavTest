
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/libopencm3/Makefile")
    message(FATAL_ERROR "The libopencm3 was not downloaded")
endif()

string(REPLACE "/" "" OUTPUT_TARGET_NAME ${LIBOPENCM3_TARGET})
string(CONCAT LIBOPENCM3_OUTPUT_NAME "libopencm3_" ${OUTPUT_TARGET_NAME} ".a")
string(CONCAT LIBOPENCM3_LIBRARY "opencm3_" ${OUTPUT_TARGET_NAME})

set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/lib/libopencm3)
add_custom_target(libopencm3_target make V=1 TARGETS=${LIBOPENCM3_TARGET} WORKING_DIRECTORY ${LIBOPENCM3_DIR})

# Create a specific CPU target with the appropriate options etc
add_library(libopencm3 STATIC IMPORTED)
set_property(TARGET libopencm3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBOPENCM3_DIR}/include)
set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${LIBOPENCM3_DIR}/lib/${LIBOPENCM3_OUTPUT_NAME})
add_dependencies(libopencm3 libopencm3_target)
target_link_directories(libopencm3 INTERFACE ${LIBOPENCM3_DIR}/lib)

target_compile_options(libopencm3 INTERFACE ${PLATFORM_CFLAGS})
target_link_options(libopencm3 INTERFACE ${PLATFORM_LDFLAGS})
