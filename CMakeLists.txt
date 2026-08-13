cmake_minimum_required(VERSION 3.22)

project(OSKengine)


#
# Librerias externas header-only.
#
include_directories(./external/)


add_library(json INTERFACE)
target_include_directories(json INTERFACE
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/json/>
                                $<INSTALL_INTERFACE:include/json/>)

add_library(stbi_image INTERFACE)
target_include_directories(stbi_image INTERFACE
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/stbi_image/>
                                $<INSTALL_INTERFACE:include/stbi_image/>)

add_library(tiny_gltf INTERFACE)
target_include_directories(tiny_gltf INTERFACE
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/tiny_gltf/>
                                $<INSTALL_INTERFACE:include/tiny_gltf/>)

#
# Librería que debemos generar para Android nativo.
# 
# Al ser compartido por dos targets (lib y exe),
# lo creamos aquí para despues compartirlo con ambos.
if(OSK_PLATFORM STREQUAL "OSK_ANDROID")
    include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
    add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    target_link_libraries(native_app_glue log android)
    add_compile_definitions(VK_USE_PLATFORM_ANDROID_KHR)
endif()

add_subdirectory(src)
add_subdirectory(demo)
