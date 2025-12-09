cmake_minimum_required(VERSION 3.31)
project(D3D11Engine)
set(CMAKE_CXX_STANDARD 20)

macro(gather_sources)
    file(GLOB INC src/*.h src/*.hpp src/**/*.h src/**/*.hpp)
    file(GLOB SRC CONFIGURE_DEPENDS src/*.cpp src/*.c src/**/*.cpp src/**/*.c)
endmacro()

gather_sources()
add_subdirectory(libs)
include_directories(libs/STB)

add_executable(${PROJECT_NAME} WIN32
        ${SRC}
        ${INC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)