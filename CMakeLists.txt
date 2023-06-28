cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

# generate an executable from our source file
add_executable(motor_control motor_control.cpp)

# link our executable with pico libraries
target_link_libraries(motor_control 
    pico_stdlib 
    hardware_pwm
    string
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(motor_control)
