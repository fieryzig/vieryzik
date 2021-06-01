cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(vieryzik)

set(VULKAN_SDK "c:/VulkanSDK")
set(VULKAN_VERSION "1.2.176.1")
set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
set(VULKAN_LIB_LIST "vulkan-1")

include_directories(AFTER ${VULKAN_PATH}/Include)
link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)

add_subdirectory(third_party/glfw)

file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(VieryziK ${CPP_FILES} ${HPP_FILES})

target_link_libraries(VieryziK ${VULKAN_LIB_LIST} glfw)

