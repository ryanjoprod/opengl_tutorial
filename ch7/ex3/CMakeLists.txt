cmake_minimum_required(VERSION 3.28)
project(opengl_ch7_ex3 VERSION 1.0)

find_package(glfw3 3.3 REQUIRED)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(EXT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/glad/src")
set(EXT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/glad/include")
add_library(glad "${EXT_INCLUDE_DIR}/glad/glad.h" "${EXT_SOURCE_DIR}/glad.c")
target_include_directories(glad PUBLIC ${EXT_INCLUDE_DIR})

target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_library(Shader src/shader.cpp)
target_link_libraries(Shader PUBLIC compiler_flags glad)

add_library(STB INTERFACE src/stb_image.h)
target_link_libraries(STB INTERFACE compiler_flags)

file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC compiler_flags glfw glad Shader STB PRIVATE ${CMAKE_DL_LIBS})
