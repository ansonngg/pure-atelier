cmake_minimum_required(VERSION 3.26)
project(DirectXECS)

set(CMAKE_CXX_STANDARD 20)

find_package(directx-headers CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/DirectXECS)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    d3d12.lib
    d3dcompiler.lib
    dxgi.lib
    dxguid.lib
    Microsoft::DirectX-Guids
    Microsoft::DirectX-Headers
)
