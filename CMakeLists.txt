cmake_minimum_required(VERSION 3.14)
project(CustomMemoryAllocators LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CustomMemoryAllocators INTERFACE)

target_include_directories(CustomMemoryAllocators INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(CustomMemoryAllocators INTERFACE cxx_std_20)

# Install headers (header-only)
install(DIRECTORY include/ DESTINATION include)

# Install the interface library target
install(TARGETS CustomMemoryAllocators
        EXPORT CustomMemoryAllocatorsTargets
        INCLUDES DESTINATION include
)

# Export the target set for find_package
install(EXPORT CustomMemoryAllocatorsTargets
        FILE CustomMemoryAllocatorsTargets.cmake
        NAMESPACE CustomMemoryAllocators::
        DESTINATION lib/cmake/CustomMemoryAllocators
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CustomMemoryAllocatorsConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/CustomMemoryAllocatorsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CustomMemoryAllocatorsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/CustomMemoryAllocators
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CustomMemoryAllocatorsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CustomMemoryAllocatorsConfigVersion.cmake"
    DESTINATION lib/cmake/CustomMemoryAllocators
)
