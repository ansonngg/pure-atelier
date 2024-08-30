cmake_minimum_required(VERSION 3.26)
project(PureAtelier)

set(CMAKE_CXX_STANDARD 20)

find_package(directx-headers REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/${CMAKE_PROJECT_NAME})

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE src/${CMAKE_PROJECT_NAME}/pch.h)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    d3d12.lib
    d3dcompiler.lib
    dxgi.lib
    dxguid.lib
    Microsoft::DirectX-Guids
    Microsoft::DirectX-Headers
    spdlog::spdlog
)
