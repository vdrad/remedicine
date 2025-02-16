# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(remedicine C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(remedicine remedicine.c 
        inc/pinout.c
        inc/rgb_matrix/rgb_matrix.c
        inc/oled_display/ssd1306_i2c.c
        inc/oled_display/oled_display.c
        inc/buzzer/buzzer.c
        inc/buttons/buttons.c
        inc/microphone/microphone.c
        inc/alarm/alarm.c
        inc/remedios/remedios.c
)

pico_set_program_name(remedicine "remedicine")
pico_set_program_version(remedicine "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(remedicine 0)
pico_enable_stdio_usb(remedicine 1)

# Add the standard library to the build
target_link_libraries(remedicine
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(remedicine PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(remedicine 
        hardware_timer
        hardware_pio
        hardware_clocks
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_adc
        hardware_rtc
        )

pico_add_extra_outputs(remedicine)

# Generate PIO header
pico_generate_pio_header(remedicine ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)