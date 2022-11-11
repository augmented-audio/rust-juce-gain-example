cmake_minimum_required(VERSION 3.14)

project(gain_plugin VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)

include(ExternalProject)
add_subdirectory(./JUCE)

ExternalProject_Add(
        rust_lib
        DOWNLOAD_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cargo build COMMAND cargo build --release
        BINARY_DIR "${CMAKE_SOURCE_DIR}/target"
        INSTALL_COMMAND ""
        LOG_BUILD ON)

juce_add_plugin(
        GainPlugin
        PLUGIN_MANUFACTURER_CODE BJFR
        PLUGIN_CODE Gain

        FORMATS AU VST3 Standalone
        PRODUCT_NAME "Gain plugin"
        ICON_BIG ./icon.png
)

target_include_directories(
        GainPlugin
        PRIVATE
        ./include/
)
target_sources(
        GainPlugin
        PRIVATE
        ./cpp/PluginEditor.cpp
        ./cpp/PluginProcessor.cpp
)
target_compile_definitions(
        GainPlugin
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set(
            RUST_LIB
            debug "${CMAKE_SOURCE_DIR}/target/aarch64-apple-ios/debug/libgain_plugin.a"
            optimized "${CMAKE_SOURCE_DIR}/target/aarch64-apple-ios/release/libgain_plugin.a"
    )
else()
    set(
            RUST_LIB
            debug "${CMAKE_SOURCE_DIR}/target/debug/libgain_plugin.a"
            optimized "${CMAKE_SOURCE_DIR}/target/release/libgain_plugin.a"
    )
endif()

target_link_libraries(
        GainPlugin
        PRIVATE
        juce::juce_audio_utils
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags

        ${RUST_LIB}
)
