cmake_minimum_required(VERSION 3.16)
project(VehicleControlECU VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5
find_package(Qt5 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

# pigpio library
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)
if(NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found! Install: sudo apt install libpigpio-dev")
endif()

# CommonAPI
find_package(CommonAPI 3.2.0 REQUIRED)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 REQUIRED)

# Generated CommonAPI code paths
if(DEFINED ENV{COMMONAPI_GEN_DIR})
    set(COMMONAPI_GEN_DIR $ENV{COMMONAPI_GEN_DIR})
    message(STATUS "Using COMMONAPI_GEN_DIR from environment: ${COMMONAPI_GEN_DIR}")
elseif(DEFINED COMMONAPI_GEN_DIR)
    message(STATUS "Using COMMONAPI_GEN_DIR from CMake argument: ${COMMONAPI_GEN_DIR}")
else()
    set(COMMONAPI_GEN_DIR ${CMAKE_SOURCE_DIR}/../../commonapi/generated)
    message(STATUS "Using default COMMONAPI_GEN_DIR: ${COMMONAPI_GEN_DIR}")
endif()

set(COMMONAPI_CORE_DIR ${COMMONAPI_GEN_DIR}/core)
set(COMMONAPI_SOMEIP_DIR ${COMMONAPI_GEN_DIR}/someip)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${COMMONAPI_CORE_DIR}
    ${COMMONAPI_SOMEIP_DIR}
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/BatteryMonitor.cpp
    src/GamepadHandler.cpp
    src/PiRacerController.cpp
    src/VehicleControlStubImpl.cpp
    lib/Adafruit_INA219.cpp
    lib/Adafruit_PCA9685.cpp
    lib/ShanwanGamepad.cpp
)

# CommonAPI generated sources
file(GLOB COMMONAPI_CORE_SOURCES
    "${COMMONAPI_CORE_DIR}/v1/vehiclecontrol/*.cpp"
)

file(GLOB COMMONAPI_SOMEIP_SOURCES
    "${COMMONAPI_SOMEIP_DIR}/v1/vehiclecontrol/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${COMMONAPI_CORE_SOURCES}
    ${COMMONAPI_SOMEIP_SOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    ${PIGPIO_LIBRARY}
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
    pthread
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(FILES config/vsomeip_ecu1.json
    DESTINATION etc
)

# Print configuration
message(STATUS "=== VehicleControlECU Configuration ===")
message(STATUS "Qt5 Core: ${Qt5Core_VERSION}")
message(STATUS "pigpio library: ${PIGPIO_LIBRARY}")
message(STATUS "CommonAPI: ${COMMONAPI_VERSION}")
message(STATUS "CommonAPI-SomeIP: ${COMMONAPI_SOMEIP_VERSION}")
message(STATUS "Generated code dir: ${COMMONAPI_GEN_DIR}")
message(STATUS "=====================================")
