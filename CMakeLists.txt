cmake_minimum_required(VERSION 3.27)
project(DirectEcs)

set(CMAKE_CXX_STANDARD 20)

find_package(directx-headers REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
else ()
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES})
endif (CMAKE_BUILD_TYPE MATCHES Debug)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/DirectEcs)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE src/DirectEcs/pch.h)

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
