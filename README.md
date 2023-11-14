# Embedded developer interview practical part

This repo contains practical part of embedded interview. Task is:

1. Develop embedded application for any of STM32 series MCU that will be capable of:
    - Receive MAVlink2 `COMMAND_LONG` messages with `MAV_CMD_USER_1` command inside and
      indicate this with changing state of onboard LED
    - Send `HEARTBEAT` messages every second
2. Develop helper APP to test main one. App should be able to:
    - Send `MAV_CMD_USER_1` every 5 seconds to the embedded software
    - Receive and indicate `HEARTBEAT` from the embedded software

# Repository and project structure

Project is based on CMake build system and can be loaded into any IDE supporting it or developed without it.

## External dependencies

Repo has external dependencies that will be downloaded during configuration phase:
- `FreeRTOS` will be downloaded to `<BUILD_DIRECTORY>/_deps` by CMake
- `libopencm3` connected as a Git submodule and may be downloaded manually or via CMake (behaviour is controlled via `GIT_SUBMODULE` option)
- `mavlink` library has same behavior as `libopencm3`

## Supported platforms and build

Firmware was tested on two platforms: `STM32F4Discovery` and `STM32VLDiscovery`. To select either of this platforms pass toolchain file from the coresponding directory:
```bash
# For manual submodule pull
#git submodule update --init --recursive
# Configure for STM32VLDiscovery platform
mkdir ./build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=./platform/stm32f407discovery/toolchain.cmake -DGIT_SUBMODULE=ON
cmake --build build
```

Platform initialization is done inside `platform.c` file and definitions are inside `platform.h`.

### Note on libopencm3 building
As it uses as external command via makefile, so sometimes CMake may not rebuild it. For manual rebuild:
```bash
make -C lib/libopencm3 clean                    # clean
make -C lib/libopencm3 V=1 TARGETS=stm32/f1     # Rebuild for stm32vldiscovery
make -C lib/libopencm3 V=1 TARGETS=stm32/f4     # Rebuild for stm32F4discovery
```


# PC utility

PC utility is writen in Python and uses `pymavlink` to generate and parse messages and `pySerial` for serial communication.s

Run it like this:
```bash
python3 test_util.py
```

Serial port settings decalred inside executable file.

## Hardware connection

On both supported platforms USART is routed to PA2/PA3 pins for TX/RX (relatively to the MCU)

# VSCode support

`.vscode` directory cotains required settings for development and debug. Next extensions are required:
- Cortex-debug
- C/C++ Extension pack

Debugging is done via `openocd`

Changing platform requires to change targets in `.vscode/launch.json`:
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "openocd",
            "request": "launch",
            "type": "cortex-debug",
            "cwd": "${workspaceRoot}",
            "servertype": "openocd",
            "executable": "${command:cmake.getLaunchTargetPath}",
            "svdFile": "platform/stm32vldiscovery/stm32f100.svd",
            // "svdFile": "./platform/stm32f407discovery/STM32F407.svd",   // <-- Uncomment for F407 platform
            "configFiles": [
                // "/usr/share/openocd/scripts/board/stm32f4discovery.cfg"
                "/usr/share/openocd/scripts/board/stm32vldiscovery.cfg"
            ]
        }
    ]
}
```
