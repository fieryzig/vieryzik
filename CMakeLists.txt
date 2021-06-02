cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(vieryzik)

find_package(Vulkan REQUIRED FATAL_ERROR)
message("FindVulkan Results: ${Vulkan_FOUND} ${Vulkan_INCLUDE_DIRS} ${Vulkan_LIBRARIES}")
include_directories(AFTER ${Vulkan_INCLUDE_DIRS})

add_subdirectory(third_party/glfw)

file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(VieryziK ${CPP_FILES} ${HPP_FILES})

target_link_libraries(VieryziK ${Vulkan_LIBRARIES} glfw)

