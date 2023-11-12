cmake_minimum_required(VERSION 3.15)

SET(CMAKE_VERBOSE_MAKEFILE          ON)
SET(CMAKE_COLOR_MAKEFILE            ON)
SET(CMAKE_ERROR_DEPRECATED          ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS   OFF)

option(GIT_SUBMODULE "Check submodules during build" OFF)   # Update submodules as needed

include(FetchContent)
find_package(Git QUIET)

project(MavTest LANGUAGES C)

set(PROJECT_MAJOR_VERSION 0)
set(PROJECT_MINOR_VERSION 1)
set(PROJECT_PATCH_VERSION 0)

set(CMAKE_C_STANDARD 11)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  if(GIT_SUBMODULE)
      message(STATUS "Submodule update")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      RESULT_VARIABLE GIT_SUBMOD_RESULT)
      if(NOT GIT_SUBMOD_RESULT EQUAL "0")
          message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
      endif()
  endif()
endif()

include(${CMAKE_SOURCE_DIR}/platform/${PLATFORM}/platform.cmake)
include(${CMAKE_SOURCE_DIR}/platform/freertos.cmake)
include(${CMAKE_SOURCE_DIR}/platform/libopencm3.cmake)
include(${CMAKE_SOURCE_DIR}/platform/mavlink.cmake)


add_link_options(
    -T ${CMAKE_SOURCE_DIR}/platform/app.ld
)

set(APP_DIR "${CMAKE_SOURCE_DIR}/app")

set(APP_SOURCES
    ${APP_DIR}/main.c
    ${APP_DIR}/hooks.c
)

add_executable(${PROJECT_NAME}.elf ${APP_SOURCES} ${PLATFORM_SOURCES})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME}.elf freertos_kernel libopencm3)